#pragma once

#include "Util.hpp"

namespace tgt::Pipe {

	enum class CullMode {
		NONE = 0,
		FRONT_BIT = 1,
		BACK_BIT = 2,
		FRONT_AND_BACK = 3
	};

	constexpr CullMode CULLMODE_MIN = CullMode::NONE;
	constexpr CullMode CULLMODE_MAX = CullMode::FRONT_AND_BACK;

	enum class PolygonMode {
		FILL = 0,
		LINE = 1,
		POINT = 2,
	};

	constexpr PolygonMode POLYGONMODE_MIN = PolygonMode::FILL;
	constexpr PolygonMode POLYGONMODE_MAX = PolygonMode::POINT;

	constexpr auto PIPE_SUBFOLDER = "Pipes";

	constexpr auto CULLMODE_PROPERTY = "cullmode";
	constexpr auto POLYGONMODE_PROPERTY = "polygonmode";
	constexpr auto SHADER_PROPERTY = "shader";
	constexpr auto INPUTBINDINGS_PROPERTY = "inputs";

	constexpr std::array SUPPORTED_PROPERTIES = { CULLMODE_PROPERTY, POLYGONMODE_PROPERTY, SHADER_PROPERTY, INPUTBINDINGS_PROPERTY };

	const auto PIPE_PATH = fs::path(Util::RESOURCE_LOCATION).append(PIPE_SUBFOLDER);

	const Result create(const std::string& name, const CullMode cullmode, const PolygonMode polygonMode);

	inline const Result remove(const std::string& name) {
		return Util::remove(PIPE_PATH, name);
	}

	inline const std::string list() {
		return Util::collect(PIPE_PATH, Util::JSON_FILTER);
	}

	const Result addShader(const std::string& name, const std::string& shadername);

	const Result removeShader(const std::string& name, const std::string& shadername);

	inline bool checkDependent(const std::string& dependency) {
		return Util::find(PIPE_PATH, [](auto& dependend) {
			js::json json;
			JSON_LOAD(dependend, json);
			return json[SHADER_PROPERTY].contains(dependend);
		});
	}

}