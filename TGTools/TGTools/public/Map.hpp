#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"

namespace tgt::Map {

	constexpr auto MAP_SUBFOLDER = "Maps";
	constexpr auto MAP_EXTENSION = ".tgr";

	constexpr auto TEXTURE_PROPERTY = "textures";
	constexpr auto ACTOR_PROPERTY = "actors";
	constexpr auto MATERIAL_PROPERTY = "materials";

	const auto MAP_PATH = fs::path(Util::RESOURCE_LOCATION).append(MAP_SUBFOLDER);

	const Result create(const char* mapname);

	const Result create(const std::string& mapname);

	const Result remove(const char* mapname);

	const Result remove(const std::string& mapname);

	const std::string list();

	const Result make(const char* mapname);

	const Result make(const std::string& mapname);

	const Result add(const char* mapname, const char* name);

	const Result add(const std::string& mapname, const std::string& name);

	const Result remove(const char* mapname, const char* name);

	const Result remove(const std::string& mapname, const std::string& name);

	const bool checkDependent(const std::string& dependency);
}