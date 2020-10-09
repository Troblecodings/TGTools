#include "../public/Sampler.hpp"

namespace tgt::Sampler {

	const Result add(const std::string& name, const SamplerAddressMode umode, const SamplerAddressMode vmode,
		const SamplerFilter magfilter, const SamplerFilter minFilter) {
		STRING_CHECKS(name);
		STRING_SYNTAX_CHECK(name);
		ENUM_CHECKS(umode, SAMPLER_ADDRESS_MODE_MIN, SAMPLER_ADDRESS_MODE_MAX);
		ENUM_CHECKS(vmode, SAMPLER_ADDRESS_MODE_MIN, SAMPLER_ADDRESS_MODE_MAX);
		ENUM_CHECKS(magfilter, SAMPLER_FILTER_MIN, SAMPLER_FILTER_MAX);
		ENUM_CHECKS(minFilter, SAMPLER_FILTER_MIN, SAMPLER_FILTER_MAX);

		auto path = Util::getResource(SAMPLER_PATH, name, Util::JSON);
		if (fs::exists(path))
			return Result::ALREADY_EXISTS;

		js::json json;
		json[UMODE_PROPERTY] = (uint32_t)umode;
		json[VMODE_PROPERTY] = (uint32_t)vmode;
		json[MAG_FILTER_PROPERTY] = (uint32_t)magfilter;
		json[MIN_FILTER_PROPERTY] = (uint32_t)minFilter;

		JSON_WRITE(path, json);
		return Result::SUCCESS;
	}

}