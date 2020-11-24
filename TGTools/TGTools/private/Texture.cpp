#include "../public/Texture.hpp"
#include "../public/Material.hpp"
#include "../public/json.hpp"

namespace tgt::Texture {

	const Result add(const std::string& path) {
		STRING_CHECKS(path);
		const fs::path texturePath(path);

		if (!fs::exists(texturePath))
			return Result::DOES_NOT_EXIST;

		auto resourceLocation = Util::getResource(TEXTURE_PATH, texturePath.stem().string(), TEXTURE_EXTENSION);
		std::error_code code;
		if (!fs::copy_file(texturePath, resourceLocation, code)) // Failing if destination exists
			return Result::ALREADY_EXISTS;

		return Result::SUCCESS;
	}

	const Result remove(const std::string& name) {
		STRING_CHECKS(name);
		STRING_SYNTAX_CHECK(name);
		return Util::remove(TEXTURE_PATH, name, TEXTURE_EXTENSION, [](const fs::path& path) { return Util::find(Material::MATERIAL_PATH, [=](auto directory) {
			nlohmann::json json;
			JSON_LOAD(directory, json);
			auto foundpath = json[Material::TEXTURE_PROPERTY].get<std::string>();
			return foundpath == path;
		}); });
	}
}