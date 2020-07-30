#include "Font.hpp"
#include "Util.hpp"

namespace tgt::Font {

	constexpr auto FONT_SUBFOLDER = "Textures";
	constexpr auto FONT_EXTENSION = ".ttf";

	const auto FONT_PATH = fs::path(Util::RESOURCE_LOCATION).append(FONT_SUBFOLDER);

	const Result add(const char* path) {
		if (*path == '\0')
			return Result::BAD_ARGUMENTS;
		const fs::path texturePath(path);

		if (!fs::exists(texturePath))
			return Result::DOES_NOT_EXIST;

		auto resourceLocation = Util::getResource(FONT_PATH, texturePath.stem().string());
		if (!fs::copy_file(texturePath, resourceLocation)) // Failing if destination exists
			return Result::ALREADY_EXISTS;

		//TODO auto generation
		return Result::SUCCESS;
	}

	const Result add(const std::string& path) {
		return add(path.c_str());
	}

	const Result remove(const char* name) {
		return remove(std::string(name));
	}

	const Result remove(const std::string& name) {
		// TODO
		return Result::GENERAL;
	}

	const std::string list() {
		return Util::collect(FONT_PATH, [](fs::path path) { return path.extension() == FONT_EXTENSION; });
	}
}