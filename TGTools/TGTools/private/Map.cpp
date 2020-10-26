#include "../public/Map.hpp"
#include "../public/json.hpp"
#include <fstream>
#include "../public/Actor.hpp"
#include "../public/Font.hpp"
#include "../public/Texture.hpp"
#include "../public/Material.hpp"
#include "../public/Buffer.hpp"
#include "../public/Sampler.hpp"
#include "../public/Pipe.hpp"
#include "../public/Shader.hpp"
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

		const js::json& textureList = mapJson[TEXTURE_PROPERTY];
		CHECK_RESULT(Texture::write(fp, textureList));

		CHECK_RESULT(Buffer::write(fp, mapJson[BUFFER_PROPERTY]));

		CHECK_RESULT(Sampler::write(fp, mapJson[SAMPLER_PROPERTY]));

		CHECK_RESULT(Material::write(fp, mapJson[MATERIAL_PROPERTY], textureList));

		CHECK_RESULT(Actor::write(fp, mapJson[ACTOR_PROPERTY]));

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