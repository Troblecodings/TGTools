#include "../public/Pipe.hpp"
#include "../public/Shader.hpp"

namespace tgt::Pipe {

	const Result create(const std::string& name, const CullMode cullmode, const PolygonMode polygonMode) {
		STRING_CHECKS(name);
		ENUM_CHECKS(cullmode, CULLMODE_MIN, CULLMODE_MAX);
		ENUM_CHECKS(polygonMode, POLYGONMODE_MIN, POLYGONMODE_MAX);

		const auto path = Util::getResource(PIPE_PATH, name, Util::JSON);
		if (fs::exists(path))
			return Result::ALREADY_EXISTS;

		js::json json;
		json[CULLMODE_PROPERTY] = cullmode;
		json[POLYGONMODE_PROPERTY] = polygonMode;
		json[SHADER_PROPERTY] = js::json::array();

		JSON_WRITE(path, json);
		return Result::SUCCESS;
	}

	const Result addShader(const std::string& name, const std::string& shadername) {
		const auto pipepath = Util::getResource(PIPE_PATH, name, Util::JSON);
		if (!fs::exists(pipepath))
			return Result::DOES_NOT_EXIST;

		const auto shaderjsonpath = Util::getResource(Shader::SHADER_PATH, name, Util::JSON).string();
		if (!fs::exists(shaderjsonpath))
			return Result::DOES_NOT_EXIST;

		Util::jsonUpdatet(pipepath, [=](js::json& json) {
			auto& shaderlist = json[SHADER_PROPERTY];
			if (shaderlist.contains(shaderjsonpath))
				return Result::ALREADY_EXISTS;
			shaderlist += shaderjsonpath;
			return Result::SUCCESS;
		});

		return Result::SUCCESS;
	}

	const Result removeShader(const std::string& name, const std::string& shadername) {
		const auto pipepath = Util::getResource(PIPE_PATH, name, Util::JSON);
		if (!fs::exists(pipepath))
			return Result::DOES_NOT_EXIST;

		const auto shaderjsonpath = Util::getResource(Shader::SHADER_PATH, name, Util::JSON).string();

		Util::jsonUpdatet(pipepath, [=](js::json& json) {
			auto& shaderlist = json[SHADER_PROPERTY];
			auto& iterator = std::remove(shaderlist.begin(), shaderlist.end(), shaderjsonpath);
			if (iterator == shaderlist.end())
				return Result::DOES_NOT_EXIST;;
			shaderlist.erase(iterator);
			return Result::SUCCESS;
		});
	}
}