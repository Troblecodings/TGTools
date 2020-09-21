#include "../public/Sampler.hpp"

namespace tgt::Sampler {

	constexpr auto SAMPLER_SUBFOLDER = "Sampler";

	constexpr auto UMODE_PROPERTY = "textures";
	constexpr auto VMODE_PROPERTY = "actors";
	constexpr auto MAG_FILTER_PROPERTY = "materials";
	constexpr auto MIN_FILTER_PROPERTY = "fonts";

	const auto SAMPLER_PATH = fs::path(Util::RESOURCE_LOCATION).append(SAMPLER_SUBFOLDER);

	const Result add(const std::string& name, const SamplerAddressMode umode, const SamplerAddressMode vmode,
		const SamplerFilter magfilter, const SamplerFilter minFilter) {
		STRING_CHECKS(name);
		ENUM_CHECKS(umode, SAMPLER_ADDRESS_MODE_MIN, SAMPLER_ADDRESS_MODE_MAX);
		ENUM_CHECKS(vmode, SAMPLER_ADDRESS_MODE_MIN, SAMPLER_ADDRESS_MODE_MAX);
		ENUM_CHECKS(magfilter, SAMPLER_FILTER_MIN, SAMPLER_FILTER_MAX);
		ENUM_CHECKS(minFilter, SAMPLER_FILTER_MIN, SAMPLER_FILTER_MAX);

		js::json json;
		json[UMODE_PROPERTY] = umode;
		json[VMODE_PROPERTY] = vmode;
		json[MAG_FILTER_PROPERTY] = magfilter;
		json[MIN_FILTER_PROPERTY] = minFilter;

		auto path = Util::getResource(SAMPLER_PATH, name, Util::JSON);
		JSON_WRITE(path, json);
	}

}