#include "../public/Shader.hpp"
#include "../public/json.hpp"
#include "../public/Buffer.hpp"
#include "../public/Pipe.hpp"
#include "../public/Sampler.hpp"
#include "../public/Texture.hpp"
#include "../../../glslang/glslang/Public/ShaderLang.h"
#include "../../../glslang/StandAlone/ResourceLimits.h"
#include "../../../glslang/StandAlone/DirStackFileIncluder.h"
#include "../../../glslang/SPIRV/GlslangToSpv.h"

namespace tgt::Shader {

	static const js::json layout(const std::string& resource, const uint32_t binding, const DescriptorType type) {
		js::json jsn;
		jsn[SHADER_LAYOUT_RESOURCE_PROPERTY] = resource;
		jsn[SHADER_LAYOUT_BINDING_PROPERTY] = binding;
		jsn[SHADER_LAYOUT_TYPE_PROPERTY] = (uint32_t)type;
		return jsn;
	}

	const Result add(const std::string& name, const ShaderType type) {
		STRING_CHECKS(name);
		auto path = Util::getResource(SHADER_PATH, name, SHADER_EXTENSION);
		auto shader = Util::getResource(SHADER_PATH, name, Util::JSON);

		if (fs::exists(path) && fs::exists(shader))
			return Result::ALREADY_EXISTS;

		js::json json;
		json[SHADER_TYPE_PROPERTY] = type;
		json[SHADER_LAYOUT_PROPERTY] = js::json::array();

		JSON_WRITE(shader, json);

		std::ofstream fileStream(path, std::ios_base::out);
		fileStream << "void main() {\n\n}";
		fileStream.close();

		return Result::SUCCESS;
	}

	const Result remove(const std::string& name) {
		const Result result = Util::remove(SHADER_PATH, name, Pipe::checkDependent);
		if (result == Result::DEPENDENT)
			return result;
		if (Util::remove(SHADER_PATH, name, SHADER_EXTENSION) == Result::DOES_NOT_EXIST)
			return result;
		return Result::SUCCESS;
	}

	const Result addStaticInput(const std::string& shader, const std::string& resource, const uint32_t binding, const DescriptorType type) {
		STRING_CHECKS(shader);
		STRING_CHECKS(resource);

		const auto path = Util::getResource(SHADER_PATH, shader, Util::JSON);
		if (!fs::exists(path))
			return Result::DOES_NOT_EXIST;

		if (type != DescriptorType::SAMPLED_IMAGE) {
			const auto bufferPath = Util::getResource(type == DescriptorType::SAMPLER ? Sampler::SAMPLER_PATH : Buffer::BUFFER_PATH, resource, Util::JSON);
			if (!fs::exists(bufferPath))
				return Result::DOES_NOT_EXIST;
		}

		return Util::jsonUpdatet(path, [=](js::json& json) {
			json[SHADER_LAYOUT_PROPERTY].push_back(layout(resource, binding, type));
			return Result::SUCCESS;
		});
	}

	constexpr EShMessages MESSAGE_OPTIONS = (EShMessages)(EShMessages::EShMsgVulkanRules & EShMessages::EShMsgDebugInfo);

	static EShLanguage getLang(ShaderType type) {
		switch (type) {
			case tgt::Shader::ShaderType::VERTEX_BIT:
				return EShLanguage::EShLangVertex;
			case tgt::Shader::ShaderType::TESSELLATION_CONTROL_BIT:
				return EShLanguage::EShLangTessControl;
			case tgt::Shader::ShaderType::TESSELLATION_EVALUATION_BIT:
				return EShLanguage::EShLangTessEvaluation;
			case tgt::Shader::ShaderType::GEOMETRY_BIT:
				return EShLanguage::EShLangGeometry;
			case tgt::Shader::ShaderType::FRAGMENT_BIT:
				return EShLanguage::EShLangFragment;
			case tgt::Shader::ShaderType::COMPUTE_BIT:
				return EShLanguage::EShLangCompute;
		}
	}

	static Result getSPIRV(const ShaderType type, const std::string& name, std::vector<unsigned int>& spirv) {
		const EShLanguage language = getLang(type);
		glslang::TShader shader(language);

		const int defaultVersion = 100;
		const uint8_t* shaderData = Util::readFile(name);
		shader.setStrings((const char* const*)&shaderData, 1);
		shader.setEnvInput(glslang::EShSource::EShSourceGlsl, language, glslang::EShClient::EShClientVulkan, defaultVersion);
		shader.setEnvClient(glslang::EShClient::EShClientVulkan, glslang::EshTargetClientVersion::EShTargetVulkan_1_2);
		shader.setEnvTarget(glslang::EShTargetLanguage::EShTargetSpv, glslang::EShTargetLanguageVersion::EShTargetSpv_1_5);

		TBuiltInResource resource = glslang::DefaultTBuiltInResource;

		if (!shader.parse(&resource, defaultVersion, false, MESSAGE_OPTIONS)) {
			printf("%s", shader.getInfoLog());
			return Result::GENERAL;
		}

		glslang::TProgram program;
		program.addShader(&shader);
		if (!program.link(MESSAGE_OPTIONS)) {
			printf("%s", program.getInfoLog());
			return Result::GENERAL;
		}
		glslang::TIntermediate* intermediate = program.getIntermediate(language);
		if (intermediate == nullptr)
			return Result::GENERAL;

		glslang::GlslangToSpv(*intermediate, spirv);
		return Result::SUCCESS;
	}

	const Result compile(const std::string& name) {
		const auto path = Util::getResource(SHADER_PATH, name, SHADER_EXTENSION);
		if (!fs::exists(path))
			return Result::DOES_NOT_EXIST;

		const auto jpath = Util::getResource(SHADER_PATH, name, Util::JSON);
		if (!fs::exists(path))
			return Result::DOES_NOT_EXIST;

		js::json json;
		JSON_LOAD(jpath, json);

		const ShaderType type = (ShaderType)json[SHADER_TYPE_PROPERTY].get<uint32_t>();

		std::vector<unsigned int> spirv;
		Result res = getSPIRV(type, path.string(), spirv);
		if (res != Result::SUCCESS)
			return res;

	}

	const Result write(FILE* fp, const js::json& jsonarry) {
		// TODO this could go wrong
		const uint32_t textureCount = std::distance(fs::directory_iterator(Texture::TEXTURE_PATH), fs::directory_iterator());

		return Util::writeToFile(fp, jsonarry, [=](const js::json& json, const std::string& name) {
			const ShaderType type = (ShaderType)json[SHADER_LAYOUT_PROPERTY].get<uint32_t>();
			fwrite(&type, sizeof(uint32_t), 1, fp);
			const js::json& layoutarr = json[SHADER_LAYOUT_PROPERTY];
			const size_t layoutsize = layoutarr.size();
			fwrite(&layoutsize, sizeof(uint32_t), 1, fp);
			// This conforms to the VkDescriptorSetLayoutBinding struct
			for (const js::json& jobj : layoutarr) {
				const uint32_t binding = jobj[SHADER_LAYOUT_BINDING_PROPERTY].get<uint32_t>();
				fwrite(&binding, sizeof(uint32_t), 1, fp);

				const DescriptorType type = (DescriptorType)jobj[SHADER_LAYOUT_TYPE_PROPERTY].get<uint32_t>();
				fwrite(&type, sizeof(DescriptorType), 1, fp);

				const uint32_t size = ([=]() { switch (type) {
					case DescriptorType::SAMPLED_IMAGE:
						return textureCount;
					case DescriptorType::SAMPLER:
					case DescriptorType::UNIFORM_BUFFER:
						return 1u;
				} })();
				fwrite(&size, sizeof(uint32_t), 1, fp);
				fwrite(&type, sizeof(ShaderType), 1, fp);
			}

			const std::string shaderPath = fs::path(name).replace_extension(SHADER_EXTENSION).string();
			size_t size;
			const uint8_t* data = Util::readFile(shaderPath, &size);
			fwrite(&size, sizeof(uint32_t), 1, fp);
			fwrite(data, sizeof(uint8_t), size, fp);
			delete[] data;
			return Result::SUCCESS;
		});
	}

}