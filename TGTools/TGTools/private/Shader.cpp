#include "../public/Shader.hpp"

namespace tgt::Shader {

	const Result add(const char* name, ShaderType type) {
		STRING_CHECKS_C(name);
		return add(std::string(name), type);
	}

	const Result add(const std::string& name, ShaderType type) {
		STRING_CHECKS(name);
		auto path = Util::getResource(SHADER_PATH, name, SHADER_EXTENSION);
		
		if (fs::exists(path))
			return Result::ALREADY_EXISTS;
		
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

		// TODO Dependencie checks

		if (!fs::remove(path))
			return Result::DOES_NOT_EXIST;

		return Result::SUCCESS;
	}

	const std::string list() {
		return Util::collect(SHADER_PATH, [](fs::path path) { return path.extension() == SHADER_EXTENSION; });
	}

}