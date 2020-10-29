#include "../public/Shader.hpp"
#include "../public/json.hpp"
#include "../public/Buffer.hpp"
#include "../public/Pipe.hpp"
#include "../public/Sampler.hpp"
#include "../public/Texture.hpp"

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