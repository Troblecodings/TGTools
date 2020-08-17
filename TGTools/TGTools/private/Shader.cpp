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

		js::json json;
		json["shaderName"] = name;
		json["shaderType"] = type;
		//json["shaderInput"] = ;
		//json["shaderOutput"] = ;

		JSON_WRITE(shader, json);
		
		std::ofstream fileStream(path, std::ios_base::out);

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