#include "../public/Actor.hpp"
#include "../public/json.hpp"
#include "../public/Map.hpp"
#include "../public/Material.hpp"

namespace tgt::Actor {

	const Result add(const char* name, const char* material) {
		STRING_CHECKS_C(name);
		STRING_CHECKS_C(material);
		return add(std::string(name), std::string(material));
	}

	const Result add(const std::string& name, const std::string& material) {
		STRING_CHECKS(name);
		STRING_CHECKS(material);
		STRING_SYNTAX_CHECK(name);

		auto actor = Util::getResource(ACTOR_PATH, name, Util::JSON);
		if (fs::exists(actor))
			return Result::ALREADY_EXISTS;

		auto materialpath = Util::getResource(Material::MATERIAL_PATH, material, Util::JSON);
		if (!fs::exists(materialpath))
			return Result::DOES_NOT_EXIST;

		js::json json;
		json[MATERIAL_PROPERTY] = materialpath;

		JSON_WRITE(actor, json);

		return Result::SUCCESS;
	}

	const Result remove(const char* name) {
		STRING_CHECKS_C(name);
		return remove(std::string(name));
	}

	const Result remove(const std::string& name) {
		STRING_CHECKS(name);
		auto actor = Util::getResource(ACTOR_PATH, name, Util::JSON);

		if (Map::checkDependent(actor.string()))
			return Result::DEPENDENT;

		if (!fs::remove(actor))
			return Result::DOES_NOT_EXIST;
		return Result::SUCCESS;
	}

	const std::string list() {
		return Util::collect(ACTOR_PATH, Util::JSON_FILTER);
	}

	const Result _dataHeader(const fs::path& name, ActorData* data) {
		if (!fs::exists(name))
			return Result::DOES_NOT_EXIST;

		js::json json;
		JSON_LOAD(name, json);

		auto jsmatrix = json[MATRIX_PROPERTY];
		for (size_t i = 0; i < 16; i++) {
			auto x = jsmatrix[i].get<float>();
			data->matrix[i] = x;
		}
		data->animationIndex = json[ANIMATION_PROPERTY];
		data->transformIndex = json[DYNAMIK_TRANSFORM_PROPERTY];
		data->material = json[MATERIAL_PROPERTY];
		data->layer = json[LAYER_PROPERTY];
		data->instanceSize = 0; // TODO
		data->instanceOffset = 0;
		data->indexDrawCount = json[INDEX_COUNT];
		data->vertexCount = json[VERTEX_COUNT];
		return Result::SUCCESS;
	}

}
