#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace tgt::Util {

	constexpr auto RESOURCE_LOCATION = "./Resource";
	constexpr auto JSON = ".json";

	template<class T, class U>
	const fs::path getResource(fs::path resource, T name,  U extension) {
		if (!fs::exists(resource))
			fs::create_directories(resource);
		if constexpr (std::is_null_pointer_v<U>) {
			return resource.append(name);
		} else {
			return resource.append(name).concat(std::string(extension));
		}
	}

	template<class T>
	const fs::path getResource(fs::path resource, T name) {
		return getResource(resource, name, nullptr);
	}

	template<class T, class U>
	const std::string collect(const T path, const U lambda) {
		fs::path directoryPath(path);
		std::string result;
		if (!fs::exists(directoryPath))
			return result;
		result.reserve(1000);
		fs::directory_iterator directory(directoryPath);
		for (auto& entry : directory) {
			auto path = entry.path();
			if (entry.is_regular_file() && lambda(path)) {
				result.append(path.string()).append("\n");
			}
		}
		result.shrink_to_fit();
		return result;
	}
}