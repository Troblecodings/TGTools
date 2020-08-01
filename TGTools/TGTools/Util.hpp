#pragma once

#include <string>
#include <filesystem>
#include <fstream>

#define JSON_WRITE(path, json) std::ofstream output(path);\
output << std::setw(4) << json << std::endl

#define JSON_LOAD(path, json) std::ifstream input(path);\
input >> json

#define JSON_UPDATE(path, update) js::json json; \
JSON_LOAD(map, json);\
update \
JSON_WRITE(map, json)


#ifndef TGT_NO_STRING_CHECKS
#define STRING_CHECKS_C(string) if(string == nullptr || *string == 0) return Result::BAD_ARGUMENTS
#define STRING_CHECKS(string) if(string.empty()) return Result::BAD_ARGUMENTS
#else
#define STRING_CHECKS_C(string)
#define STRING_CHECKS(string)
#endif // !TGT_NO_STRING_CHECKS

namespace fs = std::filesystem;

namespace tgt::Util {

	constexpr auto RESOURCE_LOCATION = "./Resource";
	constexpr auto JSON = ".json";

	constexpr auto JSON_FILTER = [](fs::path path) { return path.extension() == JSON; };

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