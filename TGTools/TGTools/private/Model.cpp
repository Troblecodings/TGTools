#include "../public/Model.hpp"

#include "../public/Texture.hpp"
#include "../public/Material.hpp"
#include "../public/Actor.hpp"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_USE_CPP14
#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_JSON
#include "../../../tinygltf/tiny_gltf.h"

namespace tgt::Model {

#define getName(text) text.name.empty() ? fs::path(text.uri).stem().string() : text.name

	static void recursive(const tinygltf::Model& model, const tinygltf::Node& node, const std::string& map = nullptr) {
		if (node.mesh == -1)
			return;
		const tinygltf::Mesh& mesh = model.meshes[node.mesh];

		uint32_t count = 0;
		int lastmaterial = -1;
		std::string actorname;
		for (const auto& primitive : mesh.primitives) {
			if (lastmaterial != primitive.material) {
				lastmaterial = primitive.material;
				const std::string materialname = model.materials[lastmaterial].name;
				actorname = mesh.name + std::to_string(count);
				count++;
				Actor::add(actorname, materialname);
			}
			if(primitive.indices == -1) {
				printf("No indicies, currently unsupported!\n");
			} else {
				const tinygltf::Accessor& accessor = model.accessors[primitive.indices];
				const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
			}
		}

		for (const auto nodeid : node.children) {
			const tinygltf::Node& newnode = model.nodes[nodeid];
			recursive(model, newnode, map);
		}
	}

	const Result loadGltf(const std::string& path, const std::string& map) {
		STRING_CHECKS(path);

		if (!fs::exists(path))
			return Result::DOES_NOT_EXIST;

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

		std::vector<std::string> imagenames;
		imagenames.reserve(model.images.size());

		for (auto& image : model.images) {
			if (image.as_is) { //TODO
				printf("Warning 'as is' not implemented, continuing!\n");
				continue;
			}

			if (!image.image.empty()) {
				std::string texturePath;
				uint32_t number = 0;
				auto textureName = getName(image);
				if (textureName.empty())
					textureName = "default";

				std::string actualName;
				do {
					actualName = textureName + std::to_string(number);
					texturePath = Util::getResource(Texture::TEXTURE_PATH, actualName, Texture::TEXTURE_EXTENSION).string();
					number++;
				} while (fs::exists(texturePath));

				imagenames.push_back(actualName);

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

			const std::string materialName = (material.name.empty() ? "default":material.name);

			std::string actualName;
			uint32_t identifier = 0;
			Result result = Result::GENERAL;

			const int source = (baseTexture == -1 ? -1:model.textures[baseTexture].source);
			const std::string texture = (source == -1 ? "":imagenames[source]);
			do {
				actualName = materialName + std::to_string(identifier);
				result = Material::add(actualName, texture, baseColor);
				identifier++;
			} while (result == Result::ALREADY_EXISTS);
		}

		tinygltf::Scene& scene = model.scenes[model.defaultScene];
		for (int i : scene.nodes) {
			tinygltf::Node& node = model.nodes[i];
			recursive(model, node, map);
		}
		return Result::SUCCESS;
	}

}