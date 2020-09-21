#pragma once

#include "Util.hpp"
#include <string>

namespace tgt::Sampler {

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
}