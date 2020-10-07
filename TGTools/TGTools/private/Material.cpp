#include "../public/Material.hpp"
#include "../public/Texture.hpp"

namespace tgt::Material {

	const Result add(const std::string& name, const std::string& texture, const uint32_t color) {
		STRING_CHECKS(name);
		STRING_SYNTAX_CHECK(name);

		auto materialPath = Util::getResource(MATERIAL_PATH, name, Util::JSON);
		if (fs::exists(materialPath))
			return Result::ALREADY_EXISTS;

		js::json json;

		if (!texture.empty()) {
			auto texturePath = Util::getResource(Texture::TEXTURE_PATH, texture, Texture::TEXTURE_EXTENSION).string();
			if (!fs::exists(texturePath))
				return Result::DOES_NOT_EXIST;
			json[TEXTURE_PROPERTY] = texturePath;
		}
		json[COLOR_PROPERTY] = color;
		JSON_WRITE(materialPath, json);

		return Result::SUCCESS;
	}
}
