#pragma once

#include <string>
#include <filesystem>
#include <fstream>

#define JSON_WRITE(path, json) std::ofstream output(path);\
output << std::setw(4) << json << std::endl

#define JSON_LOAD(path, json) std::ifstream input(path);\
input >> json

#define JSON_UPDATE(path, update) js::json json; \
JSON_LOAD(path, json);\
update \
JSON_WRITE(path, json)

#define REMOVE_IF_FOUND(name, jobj, needle) auto name##json = jobj;\
	auto name##pred = std::remove(name##json.begin(), name##json.end(), path);\
	auto name##found = name##pred == name##json.end();\
	if (name##found)\
		name##json.erase(name##pred)

#define STRING_SYNTAX_CHECK(string) for (char x : string) if (x < 48 || (x > 57 && x < 65) || (x > 90 && x < 97) || x > 122) return Result::BAD_STRING;

#ifndef TGT_NO_STRING_CHECKS
#define STRING_CHECKS_C(string) if(string == nullptr || *string == 0) return Result::BAD_ARGUMENTS
#define STRING_CHECKS(string) if(string.empty()) return Result::BAD_ARGUMENTS
#else
#define STRING_CHECKS_C(string)
#define STRING_CHECKS(string)
#endif // !TGT_NO_STRING_CHECKS

#define ID_OF(iter, pred) std::distance(iter.begin(), std::find(iter.begin(), iter.end(), pred))

namespace fs = std::filesystem;

namespace tgt::Util {

	constexpr auto RESOURCE_LOCATION = "./Resource";
	constexpr auto JSON = ".json";

	constexpr auto JSON_FILTER = [](fs::path path) { return path.extension() == JSON; };

	// We all love Andrei Alexandrescu
	template <typename T>
	class scope_exit {
	
	private:
		T lambda;

	public:
		scope_exit(T x) : lambda{ x } {}
		~scope_exit() { lambda(); }
	};


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


	inline const uint8_t* readFile(std::string& name, size_t* sizeptr = nullptr) {
		std::ifstream input(name, std::ios_base::binary | std::ios_base::ate | std::ios_base::in);
		auto size = (size_t)input.tellg();
		if (sizeptr != nullptr)
			*sizeptr = size;
		uint8_t* data = new uint8_t[size];
		input.seekg(0, std::ios_base::beg);
		input.read((char*)data, size);
		return data;
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

	template<class T, class U>
	const bool find(const T path, const U lambda) {
		fs::directory_iterator directory(path);
		for (auto& entry : directory)
			if (lambda(entry))
				return true;
		return false;
	}
}