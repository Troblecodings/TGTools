#include "../public/Map.hpp"
#include "../public/json.hpp"
#include <fstream>
#include "../public/Actor.hpp"
#include "../public/Font.hpp"
#include "../public/Texture.hpp"
#include "../public/Material.hpp"
#include <algorithm>

namespace tgt::Map {

	namespace js = nlohmann;

	const Result create(const char* mapname) {
		STRING_CHECKS_C(mapname);
		return create(std::string(mapname));
	}

	const Result create(const std::string& mapname) {
		STRING_CHECKS(mapname);
		STRING_SYNTAX_CHECK(mapname);

		auto map = Util::getResource(MAP_PATH, mapname, Util::JSON);
		if (fs::exists(map))
			return Result::ALREADY_EXISTS;

		js::json json;
		json[ACTOR_PROPERTY] = js::json::array();
		json[MATERIAL_PROPERTY] = js::json::array();
		json[TEXTURE_PROPERTY] = js::json::array();

		JSON_WRITE(map, json);
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

#define CHECK_RESULT(statement) result = statement; if(result != Result::SUCCESS) return result

	static void textureToMapFile(FILE* fp, const js::json& map) {
		auto textureList = map[TEXTURE_PROPERTY];
		auto size = textureList.size();
		fwrite(&size, sizeof(uint32_t), 1, fp);
		for (const auto& texturePath : textureList) {
			const uint8_t* data = Util::readFile(texturePath.get<std::string>(), &size);
			fwrite(&size, sizeof(size_t), 1, fp);
			fwrite(data, sizeof(uint8_t), size, fp);
		}
	}

	static Result materialToMapFile(FILE* fp, const js::json& map) {
		auto materialList = map[MATERIAL_PROPERTY];
		auto textureList = map[TEXTURE_PROPERTY];
		auto size = materialList.size();
		fwrite(&size, sizeof(uint32_t), 1, fp);
		for (const auto& jsonPath : materialList) {
			js::json json;
			JSON_LOAD(jsonPath.get<std::string>(), json);
			uint32_t textureID = ID_OF(textureList, json[Material::TEXTURE_PROPERTY].get<std::string>());
			fwrite(&textureID, sizeof(uint32_t), 1, fp);
			auto color = json[Material::COLOR_PROPERTY].get<uint32_t>();
			fwrite(&color, sizeof(uint32_t), 1, fp);
		}
		return Result::SUCCESS;
	}

	struct ActorData {
		float    matrix[16];
		uint32_t animationIndex;
		uint32_t transformIndex;
		uint32_t material;
		uint32_t layer;
		uint32_t instanceCount;
		uint32_t instanceOffset;
		uint32_t indexCount;
		uint32_t vertexCount;
	};

	const Result make(const std::string& mapname) {
		STRING_CHECKS(mapname);

		auto mapPath = Util::getResource(MAP_PATH, mapname, Util::JSON);
		if (!fs::exists(mapPath))
			return Result::DOES_NOT_EXIST;

		js::json mapJson;
		JSON_LOAD(mapPath, mapJson);

		auto map = Util::getResource(MAP_PATH, mapname, MAP_EXTENSION).string();
		FILE* fp = fopen(map.c_str(), "w");
		if (!fp)
			return Result::GENERAL;

		Result result;
		Util::scope_exit onexit([=, rs = &result]() { fclose(fp); if(*rs != Result::SUCCESS) fs::remove(map); });

		textureToMapFile(fp, mapJson);

		CHECK_RESULT(materialToMapFile(fp, mapJson));

		return Result::GENERAL;
	}

	const Result add(const char* mapname, const char* name) {
		STRING_CHECKS_C(mapname);
		STRING_CHECKS_C(name);
		return add(std::string(mapname), std::string(name));
	}

	const Result add(const std::string& mapname, const std::string& name) {
		STRING_CHECKS(mapname);
		STRING_CHECKS(name);
		
		auto map = Util::getResource(MAP_PATH, mapname, Util::JSON);
		if (!fs::exists(map))
			return Result::DOES_NOT_EXIST;

		auto path = Util::getResource(Actor::ACTOR_PATH, name, Util::JSON);
		if (!fs::exists(path))
			return Result::DOES_NOT_EXIST;
		
		js::json actor;
		JSON_LOAD(path, actor);

		auto materialPath = actor[Actor::MATERIAL_PROPERTY].get<std::string>();
		js::json material;
		JSON_LOAD(materialPath, material);

		JSON_UPDATE(map, {
			json[ACTOR_PROPERTY] += path.string(); 
			json[MATERIAL_PROPERTY] += materialPath;
			json[TEXTURE_PROPERTY] += material[Material::TEXTURE_PROPERTY].get<std::string>();
			auto fontPath = Util::getResource(Font::FONT_PATH, name, Font::FONT_EXTENSION);
			if(fs::exists(fontPath))
				json[FONT_PROPERTY] += materialPath;
		});
		return Result::SUCCESS;
	}

	const Result remove(const char* mapname, const char* name) {
		STRING_CHECKS_C(mapname);
		STRING_CHECKS_C(name);
		return remove(std::string(mapname), std::string(name));
	}

	const Result remove(const std::string& mapname, const std::string& name) {
		STRING_CHECKS(mapname);
		STRING_CHECKS(name);

		auto map = Util::getResource(MAP_PATH, mapname, Util::JSON);
		if (!fs::exists(map))
			return Result::DOES_NOT_EXIST;

		auto path = Util::getResource(Actor::ACTOR_PATH, name, Util::JSON).string();
		js::json actor;
		JSON_LOAD(path, actor);

		auto materialPath = actor[Actor::MATERIAL_PROPERTY].get<std::string>();
		js::json material;
		JSON_LOAD(materialPath, material);

		JSON_UPDATE(map, {
			auto actorArray = json[ACTOR_PROPERTY];
			auto materialArray = json[TEXTURE_PROPERTY];
			auto textureArray = json[MATERIAL_PROPERTY];
			auto fontArray = json[FONT_PROPERTY];
			REMOVE_IF_FOUND(actor, actorArray, path);
		    REMOVE_IF_FOUND(material, materialArray, materialPath);
		    REMOVE_IF_FOUND(texture, textureArray, material[Material::TEXTURE_PROPERTY].get<std::string>());
			REMOVE_IF_FOUND(font, fontArray, Util::getResource(Font::FONT_PATH, name, Font::FONT_EXTENSION));
			if (!(actorfound || materialfound || texturefound))
				return Result::DOES_NOT_EXIST;
			});

		return Result::SUCCESS;
	}

	const bool checkDependent(const std::string& dependency) {
		return Util::find(MAP_PATH, [=](auto& directory) {
			js::json json;
			JSON_LOAD(directory.path(), json);
			return json.contains(dependency);
		});
	}
}