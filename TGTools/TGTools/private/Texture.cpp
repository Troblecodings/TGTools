#include "../public/Texture.hpp"

namespace tgt::Texture {
	
	const Result add(const char* path) {
		STRING_CHECKS_C(path);
		const fs::path texturePath(path);

		if (!fs::exists(texturePath))
			return Result::DOES_NOT_EXIST;

		auto resourceLocation = Util::getResource(TEXTURE_PATH, texturePath.stem().concat(TEXTURE_EXTENSION).string());
		if(!fs::copy_file(texturePath, resourceLocation)) // Failing if destination exists
			return Result::ALREADY_EXISTS;

		return Result::SUCCESS;
	}

	const Result add(const std::string& path) {
		STRING_CHECKS(path);
		return add(path.c_str());
	}

	const Result remove(const char* name) {
		STRING_CHECKS_C(name);
		return remove(std::string(name));
	}

	const Result remove(const std::string& name) {
		STRING_CHECKS(name);
		// TODO
		return Result::GENERAL;
	}

	const std::string list() {
		return Util::collect(TEXTURE_PATH, [](fs::path path) { return path.extension() == TEXTURE_EXTENSION; });
	}

}