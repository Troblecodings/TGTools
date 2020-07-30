#pragma once

#include "Result.hpp"
#include <string>

namespace tgt::Font {

	const Result add(const char* path);

	const Result add(const std::string& path);

	const Result remove(const char* name);

	const Result remove(const std::string& name);

	const std::string list();
}