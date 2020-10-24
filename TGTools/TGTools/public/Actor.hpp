#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"
#include <array>
#include "../public/Map.hpp"

namespace tgt::Actor {

	constexpr auto ACTOR_SUBFOLDER = "Actors";
	constexpr auto ACTOR_VERTEX_EXTENSION = "tgvmdl";
	constexpr auto ACTOR_INDEX_EXTENSION = "tgimdl";

	constexpr auto MATRIX_PROPERTY = "matrix";
	constexpr auto ANIMATION_PROPERTY = "animationIndex";
	constexpr auto DYNAMIK_TRANSFORM_PROPERTY = "transformIndex";
	constexpr auto MATERIAL_PROPERTY = "material";
	constexpr auto LAYER_PROPERTY = "layer";
	constexpr auto INDEX_COUNT = "indexDrawCount";
	constexpr auto VERTEX_COUNT = "vertexCount";

	constexpr std::array SUPPORTED_PROPERTIES = { MATRIX_PROPERTY, ANIMATION_PROPERTY, 
		DYNAMIK_TRANSFORM_PROPERTY, MATERIAL_PROPERTY, LAYER_PROPERTY, INDEX_COUNT, VERTEX_COUNT };

	struct ActorData {
		float    matrix[16];
		uint32_t animationIndex;
		uint32_t transformIndex;
		uint32_t material;
		uint32_t layer;
		uint32_t instanceSize;
		uint32_t instanceOffset;
		uint32_t indexDrawCount;
		uint32_t vertexCount;
	};

	const auto ACTOR_PATH = fs::path(Util::RESOURCE_LOCATION).append(ACTOR_SUBFOLDER);

	const Result add(const std::string& name, const std::string& material);

	inline const Result remove(const std::string& name) {
		return Util::remove(ACTOR_PATH, name, Map::checkDependent);
	}

	inline const std::string list() {
		return Util::collect(ACTOR_PATH, Util::JSON_FILTER);
	}

	template<class T, typename = std::enable_if_t<Util::_validJson<T>>>
	inline const Result change(const std::string& actorname, const std::string& key, const T& value) {
		const auto actor = Util::getResource(ACTOR_PATH, actorname, Util::JSON);
		return Util::change(actor, key, value, SUPPORTED_PROPERTIES);
	}

	const Result _dataHeader(const fs::path& name, ActorData* data);

	enum class ActorDataType {
		INDEX,
		VERTEX
	};

	constexpr std::array ACTOR_DATA_TYPE_EXTENSIONS = { ACTOR_INDEX_EXTENSION, ACTOR_VERTEX_EXTENSION };

	inline Result setData(const std::string& name, const ActorDataType datatype, const void* data, const uint32_t byteSize, const bool append = false) {
		const fs::path pathToActor = Util::getResource(Actor::ACTOR_SUBFOLDER, name);
		if (!fs::exists(pathToActor))
			return Result::DOES_NOT_EXIST;
		const std::string pathToDataSet = Util::getResource(Actor::ACTOR_SUBFOLDER, name, 
			ACTOR_DATA_TYPE_EXTENSIONS[(size_t)datatype]).string();
		FILE* fp = fopen(pathToDataSet.c_str(), append ? "ab" : "wb");
		fwrite(data, sizeof(uint8_t), byteSize, fp);
		fclose(fp);
		return Result::SUCCESS;
	}

	inline const Result getData(const std::string& name, const ActorDataType datatype, const void** data, size_t* ptr = nullptr) {
		const fs::path pathToDataSet = Util::getResource(Actor::ACTOR_SUBFOLDER, name,
			ACTOR_DATA_TYPE_EXTENSIONS[(size_t)datatype]);
		if (!fs::exists(pathToDataSet))
			return Result::DOES_NOT_EXIST;
		*data = Util::readFile(pathToDataSet.string(), ptr);
		if (data == nullptr)
			return Result::GENERAL;
		return Result::SUCCESS;
	}

	const Result write(FILE* file, const js::json& jsonarray);
}