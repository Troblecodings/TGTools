#include "../public/Model.hpp"

#include "../public/Texture.hpp"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_USE_CPP14
#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_JSON
#include "../../../tinygltf/tiny_gltf.h"

namespace tgt::Model {
	
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
			if (texture.source < 0) continue;

			auto& image = model.images[texture.source];

			if (image.as_is) {
				printf("Warning 'as is' not implemented, continuing!");
				continue;
			}

			auto textureName = texture.name.empty() ? fs::path(image.uri).stem().string() : texture.name;
			if (!image.image.empty()) {
				auto texturePath = Util::getResource(Texture::TEXTURE_PATH, textureName, Texture::TEXTURE_EXTENSION).string();
				if (fs::exists(texturePath))
					printf("Warning texture already exists overriding!\n");

				if (image.component != 4)
					printf("Warning less then 4 channels are not supported\n");
				if (!stbi_write_png(texturePath.c_str(), image.width, image.height, 4, image.image.data(), image.width * 4))
					printf("Warning couldn't write png file %s \n", texturePath.c_str());
			} else {
				printf("No image data provided! Unsupported Skipping\n");
			}
		}
		return Result::SUCCESS;
	}

}