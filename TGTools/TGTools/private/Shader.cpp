#include "../public/Shader.hpp"
#include "../public/json.hpp"
#include "../public/Pipe.hpp"

namespace tgt::Shader {

	const Result add(const std::string& name, ShaderType type) {
		STRING_CHECKS(name);
		auto path = Util::getResource(SHADER_PATH, name, SHADER_EXTENSION);
		auto shader = Util::getResource(SHADER_PATH, name, Util::JSON);

		if (fs::exists(path) && fs::exists(shader))
			return Result::ALREADY_EXISTS;

		js::json json;
		json["shaderType"] = type;

		JSON_WRITE(shader, json);

		std::ofstream fileStream(path, std::ios_base::out);
		fileStream << "void main() {\n\n}";
		fileStream.close();

		return Result::SUCCESS;
	}

	const Result remove(const std::string& name) {
		STRING_CHECKS(name);

		auto path = Util::getResource(SHADER_PATH, name, SHADER_EXTENSION);
		auto shader = Util::getResource(SHADER_PATH, name, Util::JSON);

		if (Pipe::checkDependent(name))
			return Result::DEPENDENT;

		if (!(fs::remove(path) && fs::remove(shader)))
			return Result::DOES_NOT_EXIST;

		return Result::SUCCESS;
	}

}