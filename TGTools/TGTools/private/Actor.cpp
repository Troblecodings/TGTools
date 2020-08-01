#include "../public/Actor.hpp"
#include "../public/json.hpp"

namespace tgt::Actor {

	namespace js = nlohmann;

	const Result add(const char* name, const char* material) {
		STRING_CHECKS_C(name);
		STRING_CHECKS_C(material);
		return add(std::string(name), std::string(material));
	}

	const Result add(const std::string& name, const std::string& material) {
		STRING_CHECKS(name);
		STRING_CHECKS(material);

		auto actor = Util::getResource(ACTOR_PATH, name, Util::JSON);
		if (fs::exists(actor))
			return Result::ALREADY_EXISTS;

		// TODO Check if material exists

		js::json json;
		json["materialName"] = material;

		JSON_WRITE(actor, json);

		return Result::SUCCESS;
	}

	const Result remove(const char* name) {
		STRING_CHECKS_C(name);

		auto actor = Util::getResource(ACTOR_PATH, name, Util::JSON);

		// TODO Dependency check

		if (!fs::remove(actor))
			return Result::DOES_NOT_EXIST;
		return Result::SUCCESS;
	}


	const Result remove(const std::string& name) {
		STRING_CHECKS(name);
		return remove(name.c_str());
	}

	const std::string list() {
		return Util::collect(ACTOR_PATH, Util::JSON_FILTER);
	}
}