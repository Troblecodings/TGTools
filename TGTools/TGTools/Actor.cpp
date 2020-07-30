#include "Actor.hpp"

namespace tgt::Actor {

	const Result add(const char* path) {
		return add(std::string(path));
	}

	const Result add(const std::string& path) {
		auto actor = Util::getResource(ACTOR_PATH, path, Util::JSON);

	}

	const Result remove(const char* name);

	const Result remove(const std::string& name);

	const std::string list();
}