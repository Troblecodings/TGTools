#include "../public/Model.hpp"

#include "../public/Texture.hpp"
#include "../public/Material.hpp"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_USE_CPP14
#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_JSON
#include "../../../tinygltf/tiny_gltf.h"

namespace tgt::Model {

#define getName(text) text.name.empty() ? fs::path(text.uri).stem().string() : text.name

	const Result loadGltf(const std::string& path) {

		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string error;
		std::string warning;

		bool retuncode = path.ends_with(".glb") ? loader.LoadBinaryFromFile(&model, &error, &warning, path) :
			loader.LoadASCIIFromFile(&model, &error, &warning, path);

		if (!error.empty()) {
			printf(error.c_str());
			return Result::GENERAL;
		}

		if (!warning.empty())
			printf(warning.c_str());

		if (!retuncode)
			return Result::GENERAL;

		for (auto& texture : model.textures) {
			if (texture.source < 0) { //TODO
				printf("Warning texture has no source!");
				continue;
			}

			auto& image = model.images[texture.source];

			if (image.as_is) { //TODO
				printf("Warning 'as is' not implemented, continuing!");
				continue;
			}

			auto textureName = getName(image);
			if (!image.image.empty()) {
				auto texturePath = Util::getResource(Texture::TEXTURE_PATH, textureName, Texture::TEXTURE_EXTENSION).string();
				if (fs::exists(texturePath))
					printf("Warning texture already exists overriding!\n");

				if (image.component != 4) //TODO
					printf("Warning less then 4 channels are not supported\n");
				if (!stbi_write_png(texturePath.c_str(), image.width, image.height, 4, image.image.data(), image.width * 4))
					printf("Warning couldn't write png file %s \n", texturePath.c_str());
			} else {
				// TODO
				printf("No image data provided! Unsupported, skipping!\n");
			}
		}

		for (auto& material : model.materials) {
			const auto& pbr = material.pbrMetallicRoughness;

			const int baseTexture = pbr.baseColorTexture.index;
			const auto& color = pbr.baseColorFactor;
			const uint32_t baseColor = ((uint32_t)(0xFF * color[0]) << 24) | ((uint32_t)(0xFF * color[1]) << 16)
				| ((uint32_t)(0xFF * color[2]) << 8) | (uint32_t)(0xFF * color[3]);

			if (baseTexture > 0) { // TODO
				printf("Warning material has no base Texture!");
				continue;
			}

			const auto& texture = model.textures[0];
			if (texture.source < 0) { // TODO
				printf("Warning texture has no source!");
				continue;
			}
			
			const auto& image = model.images[texture.source];
			const auto textureName = getName(image);

			const Result result = Material::add(material.name, textureName, baseColor);
			if (result != Result::SUCCESS)
				printf("Warning couldn't add Material! Error %i!", (int)result);
		}

		return Result::SUCCESS;
	}

}