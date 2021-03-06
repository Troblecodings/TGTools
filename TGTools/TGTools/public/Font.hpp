#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"
#include "Texture.hpp"

namespace tgt::Font {

	constexpr auto FONT_SUBFOLDER = "Font";
	constexpr auto FONT_EXTENSION = ".tgf";

	const auto FONT_PATH = fs::path(Util::RESOURCE_LOCATION).append(FONT_SUBFOLDER);

	const Result add(const std::string& path);

	inline const Result remove(const std::string& name) {
		STRING_CHECKS(name);
		STRING_SYNTAX_CHECK(name);

		const auto font = Util::getResource(FONT_PATH, name, FONT_EXTENSION);
		const auto texture = Util::getResource(Texture::TEXTURE_PATH, name, Texture::TEXTURE_EXTENSION).string();

		const bool fontremove = fs::remove(font);
		const bool texremove = fs::remove(texture);

		if (!fontremove && !texremove)
			return Result::DOES_NOT_EXIST;

		return Result::SUCCESS;
	}

	inline const std::string list() {
		return Util::collect(FONT_PATH, [](fs::path path) { return path.extension() == FONT_EXTENSION; });
	}
}