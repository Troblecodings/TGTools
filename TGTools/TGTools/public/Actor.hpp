#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"
#include "../public/json.hpp"

namespace tgt::Actor {

	namespace js = nlohmann;

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
		uint32_t instanceCount;
		uint32_t instanceOffset;
	};

	const auto ACTOR_PATH = fs::path(Util::RESOURCE_LOCATION).append(ACTOR_SUBFOLDER);

	const Result add(const char* name, const char* material);

	const Result add(const std::string& name, const std::string& material);

	const Result remove(const char* name);

	const Result remove(const std::string& name);

	const std::string list();

	template<class T>
	constexpr bool _validString = std::is_same_v<T, std::string> || std::is_same_v<T, const std::string>
		|| std::is_same_v<T, char*> || std::is_same_v<T, const char*>;

	template<class T>
	constexpr bool _validJson = std::is_arithmetic_v<T> || _validString<T>;
	
	template<class T, class U, std::enable_if_t<_validJson<T> && _validString<U>, int> = 0>
	const Result change(const U name, const U key, T value) {
		auto actor = Util::getResource(ACTOR_PATH, name, Util::JSON);
		if (!fs::exists(actor))
			return Result::DOES_NOT_EXIST;

		if (std::find(SUPPORTED_PROPERTIES.begin(), SUPPORTED_PROPERTIES.end(), key) == SUPPORTED_PROPERTIES.end())
			return Result::UNSUPPORTED;

		JSON_UPDATE(actor, json[key] = value;);
		return Result::SUCCESS;
	}

}