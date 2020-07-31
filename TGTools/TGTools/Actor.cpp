#include "Actor.hpp"
#include "json.hpp"
#include <fstream>

namespace tgt::Actor {

	namespace js = nlohmann;

	const Result add(const char* name, const char* material) {
		return add(std::string(name), std::string(material));
	}

	const Result add(const std::string& name, const std::string& material) {
		auto actor = Util::getResource(ACTOR_PATH, name, Util::JSON);
		if (fs::exists(actor))
			return Result::ALREADY_EXISTS;

		// TODO Check if material exists

		js::json json;
		json["materialName"] = material;

		std::ofstream output(actor);
		output << std::setw(4) << json << std::endl;

		return Result::SUCCESS;
	}

	const Result remove(const char* name) {
		auto actor = Util::getResource(ACTOR_PATH, name, Util::JSON);

		// TODO Dependency check

		if (!fs::remove(actor))
			return Result::DOES_NOT_EXIST;
		return Result::SUCCESS;
	}


	const Result remove(const std::string& name) {
		return remove(name.c_str());
	}

	const std::string list() {
		return Util::collect(ACTOR_PATH, Util::JSON_FILTER);
	}
}