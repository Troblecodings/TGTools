#pragma once

#include "Result.hpp"
#include <string>

namespace tgt::Texture {

	const Result add(const char* path);

	const Result add(const std::string& path);

	const Result add(const char** paths, const size_t count);

	const Result add(const std::string* paths, const size_t count);

	const Result remove(const char* name);

	const Result remove(const std::string& name);

	const Result list(const char* result);

	const Result list(const std::string& result);
}