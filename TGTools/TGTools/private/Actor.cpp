#include "../public/Actor.hpp"
#include "../public/json.hpp"
#include "../public/Material.hpp"

namespace tgt::Actor {

	const Result add(const std::string& name, const std::string& material) {
		STRING_CHECKS(name);
		STRING_CHECKS(material);
		STRING_SYNTAX_CHECK(name);

		auto actor = Util::getResource(ACTOR_PATH, name, Util::JSON);
		if (fs::exists(actor))
			return Result::ALREADY_EXISTS;

		auto materialpath = Util::getResource(Material::MATERIAL_PATH, material, Util::JSON).string();
		if (!fs::exists(materialpath))
			return Result::DOES_NOT_EXIST;

		js::json json;
		json[MATERIAL_PROPERTY] = materialpath;

		JSON_WRITE(actor, json);

		return Result::SUCCESS;
	}

	const Result write(FILE* file, const js::json& jsonarray) {
		return Util::writeToFile(file, jsonarray, [=](const js::json& json, const std::string& name) {
			size_t size;
			uint8_t* vertexdataptr;
			RETURN_ON_FAIL_N_DEL(Actor::getData(name, ActorDataType::VERTEX, (const void**)&vertexdataptr, &size), delete[] vertexdataptr;, vertex);
			fwrite(&size, sizeof(uint32_t), 1, file);

			Actor::ActorData data;
			auto jsmatrix = json[MATRIX_PROPERTY];
			for (size_t i = 0; i < 16; i++) {
				auto x = jsmatrix[i].get<float>();
				data.matrix[i] = x;
			}
			data.animationIndex = json[ANIMATION_PROPERTY];
			data.transformIndex = json[DYNAMIK_TRANSFORM_PROPERTY];
			data.material = json[MATERIAL_PROPERTY];
			data.layer = json[LAYER_PROPERTY];
			data.instanceSize = 0; // TODO
			data.instanceOffset = 0;
			data.indexDrawCount = json[INDEX_COUNT];
			data.vertexCount = json[VERTEX_COUNT];
			fwrite(&data, sizeof(data), 1, file);

			uint8_t* indexptr;
			size_t indexSize = 0;
			RETURN_ON_FAIL_N_DEL(Actor::getData(name, ActorDataType::INDEX, (const void**)&indexptr, &indexSize), delete[] indexptr;, index);

			if (indexSize != sizeof(uint32_t) * data.indexDrawCount)
				return Result::GENERAL;

			fwrite(&indexptr, sizeof(uint32_t), data.indexDrawCount, file);

			fwrite(vertexdataptr, sizeof(uint8_t), size, file);
			return Result::SUCCESS;
		});
	}

}
