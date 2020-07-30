#include "Texture.hpp"

namespace tgt::Texture {

	const Result add(const char* path) {
		if (path == nullptr) {
			return Result::BAD_ARGUMENTS;
		}
		return Result::SUCCESS;
	}

}