#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"

namespace tgt::Actor {

	constexpr auto ACTOR_SUBFOLDER = "Actors";

	const auto ACTOR_PATH = fs::path(Util::RESOURCE_LOCATION).append(ACTOR_SUBFOLDER);

	const Result add(const char* path);

	const Result add(const std::string& path);

	const Result remove(const char* name);

	const Result remove(const std::string& name);

	const std::string list();
}