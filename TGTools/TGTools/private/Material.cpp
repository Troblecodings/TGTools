#include "../public/Material.hpp"
#include "../public/Texture.hpp"
#include "../public/json.hpp"

namespace tgt::Material {

	namespace js = nlohmann;

	const Result add(const char* name, const char* texture, const int color) {
		STRING_CHECKS_C(name);
		STRING_CHECKS_C(texture);
		return add(std::string(name), std::string(texture), color);
	}

	const Result add(const std::string& name, const std::string& texture, const int color) {
		STRING_CHECKS(name);
		STRING_CHECKS(texture);
		STRING_SYNTAX_CHECK(name);

		auto materialPath = Util::getResource(MATERIAL_PATH, name, Util::JSON);
		if (fs::exists(materialPath))
			return Result::ALREADY_EXISTS;

		auto texturePath = Util::getResource(Texture::TEXTURE_PATH, texture, Texture::TEXTURE_EXTENSION);
		if (!fs::exists(texturePath))
			return Result::DOES_NOT_EXIST;

		js::json json;
		json[TEXTURE_PROPERTY] = texturePath;
		json[COLOR_PROPERTY] = color;
		JSON_WRITE(materialPath, json);

		return Result::SUCCESS;
	}

	const std::string list() {
		return Util::collect(MATERIAL_PATH, Util::JSON_FILTER);
	}

	const Result remove(const char* name) {
		STRING_CHECKS_C(name);
		return remove(std::string(name));
	}

	const Result remove(const std::string& name) {
		STRING_CHECKS(name);

		auto path = Util::getResource(MATERIAL_PATH, name, Util::JSON);

		if (!fs::remove(path))
			return Result::DOES_NOT_EXIST;

		return Result::SUCCESS;
	}
}
