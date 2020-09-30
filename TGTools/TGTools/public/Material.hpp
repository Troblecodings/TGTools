#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"
#include <array>

namespace tgt::Material {

	constexpr auto MATERIAL_SUBFOLDER = "Materials";

	constexpr uint32_t DEFAULT_COLOR = 0xFFFFFFFF;

	constexpr auto TEXTURE_PROPERTY = "texture";
	constexpr auto COLOR_PROPERTY = "color";

	constexpr std::array SUPPORTED_PROPERTIES = { TEXTURE_PROPERTY, COLOR_PROPERTY };

	const auto MATERIAL_PATH = fs::path(Util::RESOURCE_LOCATION).append(MATERIAL_SUBFOLDER);

	const Result add(const std::string& name, const std::string& texture, const uint32_t color = DEFAULT_COLOR);

	inline const std::string list() {
		return Util::collect(MATERIAL_PATH, Util::JSON_FILTER);
	}

	inline const Result remove(const std::string& name) {
		return Util::remove(MATERIAL_PATH, name);
	}
}
