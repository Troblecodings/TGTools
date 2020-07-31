#pragma once

#include "Map.hpp"
#include "Util.hpp"

namespace tgt::Map {

	const Result create(const char* mapname);

	const Result create(const std::string& mapname);

	const Result remove(const char* mapname);

	const Result remove(const std::string& mapname);

	const std::string list();

	const Result make();

	const Result add(const char* mapname, const char* name);

	const Result add(const std::string& mapname, const std::string& name);

	const Result remove(const char* mapname, const char* name);

	const Result remove(const std::string& mapname, const std::string& name);
}