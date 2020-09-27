#include "../public/Pipe.hpp"

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

}