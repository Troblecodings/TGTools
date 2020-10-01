#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"
#include <array>

namespace tgt::Map {

	constexpr auto MAP_SUBFOLDER = "Maps";
	constexpr auto MAP_EXTENSION = ".tgr";

	constexpr auto TEXTURE_PROPERTY = "textures";
	constexpr auto SAMPLER_PROPERTY = "sampler";
	constexpr auto BUFFER_PROPERTY = "buffer";
	constexpr auto PIPE_PROPERTY = "pipes";
	constexpr auto SHADER_PROPERTY = "shader";
	constexpr auto ACTOR_PROPERTY = "actors";
	constexpr auto MATERIAL_PROPERTY = "materials";
	constexpr auto FONT_PROPERTY = "fonts";

	constexpr std::array SUPPORTED_PROPERTIES = {
		TEXTURE_PROPERTY,
		SAMPLER_PROPERTY,
		BUFFER_PROPERTY,
		PIPE_PROPERTY,
		SHADER_PROPERTY,
		ACTOR_PROPERTY,
		MATERIAL_PROPERTY,
		FONT_PROPERTY
	};

	const auto MAP_PATH = fs::path(Util::RESOURCE_LOCATION).append(MAP_SUBFOLDER);

	const Result create(const std::string& mapname);

	inline const Result remove(const std::string& mapname) {
		return Util::remove(MAP_PATH, mapname);
	}

	inline const std::string list() {
		return Util::collect(MAP_PATH, Util::JSON_FILTER);
	}

	const Result make(const std::string& mapname);

	const Result add(const std::string& mapname, const std::string& name);

	const Result remove(const std::string& mapname, const std::string& name);

	inline const bool checkDependent(const std::string& dependency) {
		return Util::find(MAP_PATH, [=](auto directory) {
			js::json json;
			JSON_LOAD(directory, json);
			return json[ACTOR_PROPERTY].contains(dependency);
		});
	}
}