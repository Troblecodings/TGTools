#include "Texture.hpp"
#include "Util.hpp"

namespace tgt::Texture {
	
	const Result add(const char* path) {
		fs::path texturePath(path);

		if (!fs::exists(texturePath))
			return Result::DOES_NOT_EXIST;

		auto resourceLocation = Util::getResource(texturePath.filename());
		if(!fs::copy_file(texturePath, resourceLocation)) // Failing if destination exists
			return Result::ALREADY_EXISTS;
	}

	const Result add(const std::string& path) {
		return add(path.c_str());
	}

	const Result add(const char** paths, const size_t count) {
		if (!paths || count == 0)
			return Result::BAD_ARGUMENTS;

		for (size_t i = 0; i < count; i++) {
			add(paths[i]);
		}
	}

	const Result add(const std::string* paths, const size_t count) {
		if (!paths || count == 0)
			return Result::BAD_ARGUMENTS;

		for (size_t i = 0; i < count; i++) {
			add(paths[i]);
		}
	}

	const Result remove(const char* name) {
		return remove(std::string(name));
	}

	const Result remove(const std::string& name);

	const Result list(const char* result);

	const Result list(const std::string& result);

}