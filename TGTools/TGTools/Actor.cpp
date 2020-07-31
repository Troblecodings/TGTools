#include "Actor.hpp"

namespace tgt::Actor {

	const Result add(const char* path) {
		return add(std::string(path));
	}

	const Result add(const std::string& path) {
		auto actor = Util::getResource(ACTOR_PATH, path, Util::JSON);
		return Result::GENERAL;
	}

	const Result remove(const char* name) {
		return Result::GENERAL;
	}


	const Result remove(const std::string& name) {
		return Result::GENERAL;
	}

	const std::string list() {
		return Util::collect(ACTOR_PATH, Util::JSON_FILTER);
	}
}