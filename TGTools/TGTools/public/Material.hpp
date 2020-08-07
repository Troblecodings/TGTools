#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"

namespace tgt::Material {

	constexpr auto DEFAULT_COLOR = 0xFFFFFFFF;

	constexpr auto MATERIAL_SUBFOLDER = "Materials";

	const auto MATERIAL_PATH = fs::path(Util::RESOURCE_LOCATION).append(MATERIAL_SUBFOLDER);

	const Result add(const char* name, const char* texture, int color = DEFAULT_COLOR);

	const Result add(const std::string& name, const std::string& texture, const int color = DEFAULT_COLOR);

	const std::string list();

	const Result remove(const char* name);

	const Result remove(const std::string& name);
}