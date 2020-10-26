#include "../public/Shader.hpp"
#include "../public/json.hpp"
#include "../public/Buffer.hpp"
#include "../public/Pipe.hpp"

namespace tgt::Shader {

	const Result add(const std::string& name, ShaderType type) {
		STRING_CHECKS(name);
		auto path = Util::getResource(SHADER_PATH, name, SHADER_EXTENSION);
		auto shader = Util::getResource(SHADER_PATH, name, Util::JSON);

		if (fs::exists(path) && fs::exists(shader))
			return Result::ALREADY_EXISTS;

		js::json json;
		json[SHADER_TYPE_PROPERTY] = type;

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

	const Result addBuffer(const std::string& shader, const std::string& buffer) {
		STRING_CHECKS(shader);
		STRING_CHECKS(buffer);

		const auto path = Util::getResource(SHADER_PATH, shader, Util::JSON);
		if (!fs::exists(path))
			return Result::DOES_NOT_EXIST;

		const auto bufferPath = Util::getResource(Buffer::BUFFER_PATH, buffer, Util::JSON);
		if (!fs::exists(bufferPath))
			return Result::DOES_NOT_EXIST;

		return Util::jsonUpdatet(path, [=](js::json& json) {
			js::json bufferJson;
			JSON_LOAD(bufferPath, bufferJson);
			return Result::SUCCESS;
		});
	}

}