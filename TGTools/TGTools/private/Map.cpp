#include "../public/Map.hpp"
#include "../public/json.hpp"
#include <fstream>
#include "../public/Actor.hpp"
#include "../public/Font.hpp"
#include "../public/Texture.hpp"
#include "../public/Material.hpp"
#include <algorithm>

namespace tgt::Map {

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

#define CHECK_RESULT(statement) result = statement; if(result != Result::SUCCESS) return result

#define WRITE_INT(fp, dt) fwrite(&dt, sizeof(uint32_t), 1, fp)
#define WRITE_CINT(fp, num) check = num; fwrite(&check, sizeof(uint32_t), 1, fp)
#define WRITE_CHECK(fp) constexpr auto WRITE_CINT(fp, UINT32_MAX);
#define WRITE_SIZE(fp) WRITE_INT(fp, size)

	static void textureToMapFile(FILE* fp, const js::json& map) {
		auto textureList = map[TEXTURE_PROPERTY];
		auto size = textureList.size();
		fwrite(&size, sizeof(uint32_t), 1, fp);
		for (const auto& texturePath : textureList) {
			const uint8_t* data = Util::readFile(texturePath.get<std::string>(), &size);
			fwrite(&size, sizeof(size_t), 1, fp);
			fwrite(data, sizeof(uint8_t), size, fp);
			delete[] data;
		}
		WRITE_CHECK(fp);
	}

	static Result materialToMapFile(FILE* fp, const js::json& map) {
		auto materialList = map[MATERIAL_PROPERTY];
		auto textureList = map[TEXTURE_PROPERTY];
		auto size = materialList.size();
		WRITE_SIZE(fp);
		for (const auto& jsonPath : materialList) {
			js::json json;
			auto materialPath = jsonPath.get<std::string>();
			JSON_LOAD(materialPath, json);
			uint32_t textureID = (uint32_t)ID_OF(textureList, json[Material::TEXTURE_PROPERTY].get<std::string>());
			WRITE_INT(fp, textureID);
			auto color = json[Material::COLOR_PROPERTY].get<uint32_t>();
			WRITE_INT(fp, color);
		}
		WRITE_CHECK(fp);
		return Result::SUCCESS;
	}

	static Result actorToMapFile(FILE* fp, const js::json& map) {
		auto actorlist = map[ACTOR_PROPERTY];
		auto size = actorlist.size();
		WRITE_SIZE(fp);
		for (const auto& jsonPath : actorlist) {
			const std::string& stringPath = jsonPath;
			fs::path stempath(stringPath);
			stempath.replace_extension(Actor::ACTOR_VERTEX_EXTENSION);
			uint8_t* vertexdataptr;
			Result CHECK_RESULT(Actor::getData((const void**)&vertexdataptr, stempath, &size));
			Util::scope_exit onexit([=]() { delete[] vertexdataptr; });
			WRITE_SIZE(fp);

			Actor::ActorData data;
			CHECK_RESULT(Actor::_dataHeader(stringPath, &data));
			fwrite(&data, sizeof(data), 1, fp);

			uint8_t* indexptr;
			size_t indexSize = 0;
			stempath.replace_extension(Actor::ACTOR_INDEX_EXTENSION);
			CHECK_RESULT(Actor::getData((const void**)&indexptr, stempath, &indexSize));

			if (indexSize != sizeof(uint32_t) * data.indexDrawCount)
				printf("Warning index size is greater then the actual draw count");

			fwrite(&indexptr, sizeof(uint32_t), data.indexDrawCount, fp);
			delete[] indexptr;

			fwrite(vertexdataptr, sizeof(uint8_t), size, fp);
		}
		WRITE_CHECK(fp);

		return Result::SUCCESS;
	}

	const Result make(const std::string& mapname) {
		STRING_CHECKS(mapname);

		auto mapPath = Util::getResource(MAP_PATH, mapname, Util::JSON);
		if (!fs::exists(mapPath))
			return Result::DOES_NOT_EXIST;

		js::json mapJson;
		JSON_LOAD(mapPath, mapJson);

		auto map = Util::getResource(MAP_PATH, mapname, MAP_EXTENSION).string();
		FILE* fp = fopen(map.c_str(), "wb");
		if (!fp)
			return Result::GENERAL;

		Result result;
		Util::scope_exit onexit([=, rs = &result]() { fclose(fp); if(*rs != Result::SUCCESS) fs::remove(map); });

		textureToMapFile(fp, mapJson);

		CHECK_RESULT(materialToMapFile(fp, mapJson));

		CHECK_RESULT(actorToMapFile(fp, mapJson));

		// TODO
		constexpr uint32_t zero = 0;
		WRITE_INT(fp, zero);
		WRITE_CHECK(fp);

		WRITE_INT(fp, zero);
		WRITE_INT(fp, check);

		WRITE_INT(fp, zero);
		WRITE_INT(fp, check);

		return Result::GENERAL;
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
				json[FONT_PROPERTY] += fontPath;
		});
		return Result::SUCCESS;
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

}