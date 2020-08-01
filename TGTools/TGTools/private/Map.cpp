#pragma once

#include "../public/Map.hpp"
#include "../public/json.hpp"
#include <fstream>
#include "../public/Actor.hpp"
#include "../public/Font.hpp"

namespace tgt::Map {

	namespace js = nlohmann;

	const Result create(const char* mapname) {
		STRING_CHECKS_C(mapname);
		return create(std::string(mapname));
	}

	const Result create(const std::string& mapname) {
		STRING_CHECKS(mapname);

		auto map = Util::getResource(MAP_PATH, mapname, Util::JSON);
		if (fs::exists(map))
			return Result::ALREADY_EXISTS;

		JSON_WRITE(map, js::json::array());
		return Result::SUCCESS;
	}

	const Result remove(const char* mapname) {
		STRING_CHECKS_C(mapname);
		return remove(std::string(mapname));
	}

	const Result remove(const std::string& mapname) {
		STRING_CHECKS(mapname);

		auto map = Util::getResource(MAP_PATH, mapname, Util::JSON);
		if (!fs::remove(map))
			return Result::DOES_NOT_EXIST;

		return Result::SUCCESS;
	}

	const std::string list() {
		return Util::collect(MAP_PATH, Util::JSON_FILTER);
	}

	const Result make(const char* mapname) {
		STRING_CHECKS_C(mapname);
		return make(std::string(mapname));
	}

	const Result make(const std::string& mapname) {
		STRING_CHECKS(mapname);

		// TODO this is going to hurt
		return Result::GENERAL;
	}

	const Result add(const char* mapname, const char* name) {
		STRING_CHECKS_C(mapname);
		STRING_CHECKS_C(name);

		auto map = Util::getResource(MAP_PATH, mapname, Util::JSON);
		if (!fs::exists(map))
			return Result::DOES_NOT_EXIST;

		auto path = Util::getResource(Actor::ACTOR_PATH, name, Util::JSON);
		if (!fs::exists(path)) {
			path = Util::getResource(Font::FONT_PATH, name, Util::JSON);
			if (!fs::exists(path))
				return Result::DOES_NOT_EXIST;
		}

		JSON_UPDATE(map, json += path.string(););

		return Result::SUCCESS;
	}

	const Result add(const std::string& mapname, const std::string& name) {
		STRING_CHECKS(mapname);
		STRING_CHECKS(name);
		return add(mapname.c_str(), name.c_str());
	}

	const Result remove(const char* mapname, const char* name) {
		STRING_CHECKS_C(mapname);
		STRING_CHECKS_C(name);

		auto map = Util::getResource(MAP_PATH, mapname, Util::JSON);
		if (!fs::exists(map))
			return Result::DOES_NOT_EXIST;

		auto path = Util::getResource(Actor::ACTOR_PATH, name, Util::JSON).string();
		auto path2 = Util::getResource(Font::FONT_PATH, name, Util::JSON).string();


		JSON_UPDATE(map, {
			auto predicate = std::remove_if(json.begin(), json.end(), [=](std::string str) { return str == path || str == path2; });
			if (predicate == json.end()) 
				return Result::DOES_NOT_EXIST;
			json.erase(predicate);
		});

		return Result::SUCCESS;
	}

	const Result remove(const std::string& mapname, const std::string& name) {
		STRING_CHECKS(mapname);
		STRING_CHECKS(name);
		return remove(mapname.c_str(), name.c_str());
	}
}