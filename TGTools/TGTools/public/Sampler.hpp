#pragma once

#include "Util.hpp"
#include <string>

namespace tgt::Sampler {

	constexpr auto SAMPLER_SUBFOLDER = "Sampler";

	constexpr auto UMODE_PROPERTY = "u_address_mode";
	constexpr auto VMODE_PROPERTY = "v_address_mode";
	constexpr auto MAG_FILTER_PROPERTY = "magnification_filter";
	constexpr auto MIN_FILTER_PROPERTY = "minification_filter";

	const auto SAMPLER_PATH = fs::path(Util::RESOURCE_LOCATION).append(SAMPLER_SUBFOLDER);

	constexpr std::array SUPPORTED_PROPERTIES = {UMODE_PROPERTY, VMODE_PROPERTY, MAG_FILTER_PROPERTY, MIN_FILTER_PROPERTY};

	enum class SamplerAddressMode {
		REPEAT = 0,
		MIRRORED_REPEAT = 1,
		CLAMP_TO_EDGE = 2,
		CLAMP_TO_BORDER = 3,
		MIRROR_CLAMP_TO_EDGE = 4
	};

	constexpr SamplerAddressMode SAMPLER_ADDRESS_MODE_MIN = SamplerAddressMode::REPEAT;
	constexpr SamplerAddressMode SAMPLER_ADDRESS_MODE_MAX = SamplerAddressMode::MIRROR_CLAMP_TO_EDGE;

	enum class SamplerFilter {
		NEAREST = 0,
		LINEAR = 1
	};

	constexpr SamplerFilter SAMPLER_FILTER_MIN = SamplerFilter::NEAREST;
	constexpr SamplerFilter SAMPLER_FILTER_MAX = SamplerFilter::LINEAR;

	const Result add(const std::string& name, const SamplerAddressMode umode, const SamplerAddressMode vmode,
		const SamplerFilter magfilter, const SamplerFilter minFilter);

	inline const Result add(const std::string& name, const SamplerAddressMode mode, const SamplerFilter filter) {
		return add(name, mode, mode, filter, filter);
	}

	inline const Result remove(const std::string& name) {
		return Util::remove(SAMPLER_PATH, name);
	}

	inline const std::string list() {
		return Util::collect(SAMPLER_PATH, Util::JSON_FILTER);
	}

	inline const Result change(const std::string& name, const std::string& key, const std::string& value) {
		return Util::change(Util::getResource(SAMPLER_PATH, name, Util::JSON), key, value, SUPPORTED_PROPERTIES);
	}

	struct SamplerInfo {
		uint8_t umode;
		uint8_t vmode;
		uint8_t magfilter;
		uint8_t minfilter;
	};

	inline const Result write(FILE* file, const js::json& jsonarray) {
		return Util::writeToFile(file, jsonarray, [=](const js::json& jsn) {
			SamplerInfo samplerInfo;
			samplerInfo.umode = (uint8_t)jsn[UMODE_PROPERTY].get<SamplerAddressMode>();
			samplerInfo.vmode = (uint8_t)jsn[VMODE_PROPERTY].get<SamplerAddressMode>();
			samplerInfo.magfilter = (uint8_t)jsn[MAG_FILTER_PROPERTY].get<SamplerFilter>();
			samplerInfo.minfilter = (uint8_t)jsn[MIN_FILTER_PROPERTY].get<SamplerFilter>();
			fwrite(&samplerInfo, 1, sizeof(SamplerInfo), file);
		});
	}

}