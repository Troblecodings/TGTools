#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"
#include <array>

namespace tgt::Actor {

	constexpr auto ACTOR_SUBFOLDER = "Actors";
	constexpr auto ACTOR_EXTENSION = "tgmdl";

	constexpr auto MATRIX_PROPERTY = "matrix";
	constexpr auto ANIMATION_PROPERTY = "animationIndex";
	constexpr auto DYNAMIK_TRANSFORM_PROPERTY = "transformIndex";
	constexpr auto MATERIAL_PROPERTY = "material";
	constexpr auto LAYER_PROPERTY = "layer";

	constexpr std::array SUPPORTED_PROPERTIES = { MATRIX_PROPERTY, ANIMATION_PROPERTY, 
		DYNAMIK_TRANSFORM_PROPERTY, MATERIAL_PROPERTY, LAYER_PROPERTY };

	struct ActorData {
		float    matrix[16];
		uint32_t animationIndex;
		uint32_t transformIndex;
		uint32_t material;
		uint32_t layer;
		uint32_t instanceSize;
		uint32_t instanceOffset;
	};

	const auto ACTOR_PATH = fs::path(Util::RESOURCE_LOCATION).append(ACTOR_SUBFOLDER);

	const Result add(const char* name, const char* material);

	const Result add(const std::string& name, const std::string& material);

	const Result remove(const char* name);

	const Result remove(const std::string& name);

	const std::string list();

	template<class T, class U, class V, 
		typename = std::enable_if_t<Util::_validJson<T> && Util::_validString<V> && Util::_validString<U>>>
	const Result change(V actorname, U key, T value) {
		auto actor = Util::getResource(ACTOR_PATH, actorname, Util::JSON);
		return Util::change(actor, key, value, SUPPORTED_PROPERTIES);
	}
	const Result _dataHeader(const std::string& name, ActorData* data);

}