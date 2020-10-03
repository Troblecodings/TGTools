#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"

namespace tgt::Texture {

	constexpr auto TEXTURE_EXTENSION = ".tgx";
	constexpr auto TEXTURE_SUBFOLDER = "Textures";

	const auto TEXTURE_PATH = fs::path(Util::RESOURCE_LOCATION).append(TEXTURE_SUBFOLDER);

	const Result add(const std::string& path) noexcept;

	const Result remove(const std::string& name);

	const std::string list();
}