#pragma once

#include "Result.hpp"
#include <string>
#include <filesystem>
#include <fstream>
#include "../public/json.hpp"

#define JSON_WRITE(path, json) std::ofstream path##Output(path);\
path##Output << std::setw(4) << json << std::endl

#define JSON_LOAD(path, json) std::ifstream path##Input(path);\
path##Input >> json

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

#ifndef TGT_NO_CHECKS
#define STRING_CHECKS_C(string) if(string == nullptr || *string == 0) { printf("String check failed in (%s -> L%i)", __FILE__, __LINE__); return Result::BAD_ARGUMENTS;}
#define STRING_CHECKS(string) if(string.empty()) { printf("String check failed in (%s -> L%i)", __FILE__, __LINE__); return Result::BAD_ARGUMENTS; }
#define ENUM_CHECKS(enm, min, max) if(enm >= min && enm <= max) { printf("Enum check failed in (%s -> L%i)", __FILE__, __LINE__); return Result::BAD_ARGUMENTS; }
#else
#define STRING_CHECKS_C(string)
#define STRING_CHECKS(string)
#define ENUM_CHECKS(enm, min, max)
#endif // !TGT_NO_STRING_CHECKS

#define ID_OF(iter, pred) std::distance(iter.begin(), std::find(iter.begin(), iter.end(), pred))
#define ID_OF_P(iter, pred) std::distance(iter.begin(), std::find_if(iter.begin(), iter.end(), pred))

namespace fs = std::filesystem;
namespace js = nlohmann;

namespace tgt::Util {

	constexpr auto RESOURCE_LOCATION = "./Resource";
	constexpr auto JSON = ".json";

	constexpr auto JSON_FILTER = [](fs::path path) { return path.extension() == JSON; };

	template<class T, class... U>
	constexpr bool _isAnyOf = std::disjunction_v<std::is_same<T, U>...>;

	template<class T>
	constexpr bool _validString = _isAnyOf<T, std::string, const std::string, char*, const char*, char* const, const char* const>;

	template<class T>
	constexpr bool _validPath = _isAnyOf<T, fs::path, const fs::path>;

	template<class T>
	constexpr bool _validPathString = _validPath<T> || _validString<T>;

	template<class T>
	constexpr bool _validJson = std::is_arithmetic_v<T> || _validString<T>;

	// We all love Andrei Alexandrescu
	template <typename T, typename = std::enable_if_t<std::is_invocable_v<T>>>
	class scope_exit {

	private:
		T lambda;

	public:
		scope_exit(T x) : lambda{ x } {}
		~scope_exit() { lambda(); }
	};

	template<class T, typename = std::enable_if_t<std::is_invocable_r_v<const Result, T, js::json&>>>
	inline const Result jsonUpdatet(const fs::path& path, T lambda) {
		JSON_UPDATE(path, const Result rst = lambda(json);  if (rst != Result::SUCCESS) return rst;);
		return Result::SUCCESS;
	}

	template<class T, class U, class S, typename = std::enable_if_t<_validJson<T>&& _validString<U>>>
	inline const Result change(fs::path path, const U& key, const T& value, const S& supported) {
		if (!fs::exists(path))
			return Result::DOES_NOT_EXIST;

		if constexpr (std::is_pointer_v<U>) {
			if (std::find_if(supported.begin(), supported.end(), [=](const char* x) { return strcmp(x, key) == 0; }) == supported.end())
				return Result::UNSUPPORTED;
		} else {
			if (std::find(supported.begin(), supported.end(), key) == supported.end())
				return Result::UNSUPPORTED;
		}

		JSON_UPDATE(path, json[key] = value;);
		return Result::SUCCESS;
	}

	template<class T, class U, class P, typename = std::enable_if_t<_validPath<P>&&
		_validString<T> && (_validString<U> || std::is_null_pointer_v<U>)>>
		inline const fs::path getResource(P resource, const T& name, const U& extension) {
		if (!fs::exists(resource))
			fs::create_directories(resource);
		if constexpr (std::is_null_pointer_v<U>) {
			return resource.append(name);
		} else {
			return resource.append(name).concat(std::string(extension));
		}
	}

	template<class T, class P, typename = std::enable_if_t<_validPath<P>&& _validString<T>>>
	inline const fs::path getResource(P resource, const T& name) {
		return getResource(resource, name, nullptr);
	}


	inline const uint8_t* readFile(const std::string& name, size_t* sizeptr = nullptr) {
		std::ifstream input(name, std::ios_base::binary | std::ios_base::ate | std::ios_base::in);
		auto size = (size_t)input.tellg();
		if (sizeptr != nullptr)
			*sizeptr = size;
		uint8_t* data = new uint8_t[size];
		input.seekg(0, std::ios_base::beg);
		input.read((char*)data, size);
		return data;
	}

	template<class T, class U, typename = std::enable_if_t<_validPath<T>&& std::is_invocable_r_v<bool, U, fs::path>>>
	inline const std::string collect(const T& path, U lambda) {
		std::string result;
		if (!fs::exists(path))
			return result;
		result.reserve(1000);
		fs::directory_iterator directory(path);
		for (auto& entry : directory) {
			auto path = entry.path();
			if (entry.is_regular_file() && lambda(path)) {
				result.append(path.string()).append("\n");
			}
		}
		result.shrink_to_fit();
		return result;
	}

	template<class T, class U, typename = std::enable_if_t<_validPath<T>&& std::is_invocable_r_v<bool, U, fs::path>>>
	inline const bool find(const T& path, const U lambda) {
		fs::directory_iterator directory(path);
		for (auto& entry : directory)
			if (lambda(entry.path()))
				return true;
		return false;
	}

	template<class T, typename = std::enable_if_t<std::is_invocable_v<T, const js::json&>>>
	inline const Result writeToFile(FILE* file, const js::json& jsonarray, T lambda) {
		const auto size = jsonarray.size();
		fwrite(&size, 1, sizeof(uint32_t), file);
		for (const auto& jobject : jsonarray) {
			const std::string& name = jobject.get<std::string>();
			if (!fs::exists(name)) {
				printf("Warning: %s does not exist!", name.c_str());
				return Result::DOES_NOT_EXIST;
			}
			js::json json;
			JSON_LOAD(name, json);
			lambda(json);
		}
		constexpr auto end = 0xFFFFFFFF;
		fwrite(&end, 1, sizeof(end), file);
	}

	template<class T, typename = std::enable_if_t<std::is_invocable_r_v<bool, T, const std::string&> ||
		std::is_invocable_r_v<bool, T, const fs::path&> || std::is_null_pointer_v<T>>>
	inline const Result remove(const fs::path& parentpath, const std::string& name, const std::string& filter, T lambda) {
		auto path = Util::getResource(parentpath, name, filter);

		if constexpr (!std::is_null_pointer_v<T>) {
			if constexpr (std::is_invocable_r_v<bool, T, const std::string&>) {
				if (lambda(path.string()))
					return Result::DEPENDENT;
			} else {
				if (lambda(path))
					return Result::DEPENDENT;
			}
		}

		if (!fs::remove(path))
			return Result::DOES_NOT_EXIST;

		return Result::SUCCESS;
	}

	template<class T, typename = std::enable_if_t<std::is_invocable_r_v<bool, T, const std::string&> ||
		std::is_invocable_r_v<bool, T, const fs::path&>>>
	inline const Result remove(const fs::path& path, const std::string& name, T lambda) {
		return remove(path, name, Util::JSON, lambda);
	}

	inline const Result remove(const fs::path& path, const std::string& name) {
		return remove(path, name, Util::JSON, nullptr);
	}
}