#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"

namespace tgt::Texture {

	constexpr auto TEXTURE_EXTENSION = ".tgx";
	constexpr auto TEXTURE_SUBFOLDER = "Textures";

	const auto TEXTURE_PATH = fs::path(Util::RESOURCE_LOCATION).append(TEXTURE_SUBFOLDER);

	const Result add(const std::string& path);

	const Result remove(const std::string& name);

	inline const std::string list() {
		return Util::collect(TEXTURE_PATH, [](fs::path path) { return path.extension() == TEXTURE_EXTENSION; });
	}

	inline const Result write(FILE* fp, const js::json& jsonarray) {
		return Util::writeToFile<true>(fp, jsonarray, [=](const std::string& name) {
			size_t size = 0;
			const uint8_t* data = Util::readFile(name, &size);
			if (data == nullptr || size == 0)
				return Result::DOES_NOT_EXIST;
			fwrite(&size, sizeof(size_t), 1, fp);
			fwrite(data, sizeof(uint8_t), size, fp);
			delete[] data;
			return Result::SUCCESS;
		});
	}

}