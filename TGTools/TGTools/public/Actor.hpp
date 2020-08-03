#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"

namespace tgt::Actor {

	constexpr auto ACTOR_SUBFOLDER = "Actors";

	const auto ACTOR_PATH = fs::path(Util::RESOURCE_LOCATION).append(ACTOR_SUBFOLDER);

	const Result add(const char* name, const char* material);

	const Result add(const std::string& name, const std::string& material);

	const Result remove(const char* name);

	const Result remove(const std::string& name);

	const std::string list();

	const Result change(const std::string& name, const std::string& key, const std::string& value);

	const Result change(const char* name, const char* key, const char* value);

}