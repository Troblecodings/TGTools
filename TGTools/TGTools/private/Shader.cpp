#include "../public/Shader.hpp"
#include "../public/json.hpp"

namespace tgt::Shader {

	const Result add(const char* name, ShaderType type) {
		STRING_CHECKS_C(name);
		return add(std::string(name), type);
	}

	const Result add(const std::string& name, ShaderType type) {
		STRING_CHECKS(name);
		auto path = Util::getResource(SHADER_PATH, name, SHADER_EXTENSION);
		auto shader = Util::getResource(SHADER_PATH, name, Util::JSON);
		
		if (fs::exists(path) && fs::exists(shader))
			return Result::ALREADY_EXISTS;

		glslang::InitializeProcess();
		
		TBuiltInResource Resource;
		glslang::TShader Shader(type); 
		auto file = Util::readFile(path.string());
		Shader.setStrings((const char *const *) (&file), 1);
		Shader.setEnvInput(glslang::EShSourceGlsl, type, glslang::EShClientVulkan, 100);
		Shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
		Shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_3);
		Shader.parse(&Resource, 460, true, EShMessages::EShMsgVulkanRules);
		printf(Shader.getInfoLog());

		glslang::FinalizeProcess();

		js::json json;
		json["shaderName"] = name;
		json["shaderType"] = type;
		//json["shaderInput"] = ;
		//json["shaderOutput"] = ;

		JSON_WRITE(shader, json);
		

		return Result::SUCCESS;
	}

	const Result remove(const char* name) {
		STRING_CHECKS_C(name);
		return remove(std::string(name));
	}

	const Result remove(const std::string& name) {
		STRING_CHECKS(name);

		auto path = Util::getResource(SHADER_PATH, name, SHADER_EXTENSION);
		auto shader = Util::getResource(SHADER_PATH, name, Util::JSON);

		// TODO Dependencie checks

		if (!(fs::remove(path) && fs::remove(shader)))
			return Result::DOES_NOT_EXIST;

		return Result::SUCCESS;
	}

	const std::string list() {
		return Util::collect(SHADER_PATH, [](fs::path path) { return path.extension() == SHADER_EXTENSION; });
	}

}