#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"

namespace tgt::Font {

	constexpr auto FONT_SUBFOLDER = "Textures";
	constexpr auto FONT_EXTENSION = ".ttf";

	const auto FONT_PATH = fs::path(Util::RESOURCE_LOCATION).append(FONT_SUBFOLDER);

	const Result add(const char* path);

	const Result add(const std::string& path);

	const Result remove(const char* name);

	const Result remove(const std::string& name);

	const std::string list();
}