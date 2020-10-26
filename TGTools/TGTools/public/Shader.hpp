#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"
#include "../../../glslang/glslang/Public/ShaderLang.h"

namespace tgt::Shader {

	using ShaderType = EShLanguage;

	namespace ShaderInput {

		using ShaderInputStride = uint32_t;

		constexpr ShaderInputStride POS = sizeof(float) * 3;
		constexpr ShaderInputStride POS_TEX = POS + sizeof(float) * 2;
		constexpr ShaderInputStride POS_NOR = POS + sizeof(float) * 3;
		constexpr ShaderInputStride POS_NOR_TEX = POS_NOR + sizeof(float) * 2;

		enum class ShaderInputType {
			POSITION,
			POSITION_UV,
			POSITION_NORMAL,
			POSITION_NORMAL_UV
		};
	};

	enum class DescriptorType {
		SAMPLER = 0,
		COMBINED_IMAGE_SAMPLER = 1,
		SAMPLED_IMAGE = 2,
		STORAGE_IMAGE = 3,
		UNIFORM_TEXEL_BUFFER = 4,
		STORAGE_TEXEL_BUFFER = 5,
		UNIFORM_BUFFER = 6,
		STORAGE_BUFFER = 7,
		UNIFORM_BUFFER_DYNAMIC = 8,
		STORAGE_BUFFER_DYNAMIC = 9,
		INPUT_ATTACHMENT = 10
	};

	constexpr auto DESCRIPTOR_TYPE_MIN = DescriptorType::SAMPLER;
	constexpr auto DESCRIPTOR_TYPE_MAX = DescriptorType::INPUT_ATTACHMENT;

	constexpr auto SHADER_EXTENSION = ".glsl";
	constexpr auto SHADER_SUBFOLDER = "Shaders";
	
	constexpr auto SHADER_TYPE_PROPERTY = "shaderType";
	constexpr auto SHADER_LAYOUT_PROPERTY = "shaderLayouts";

	constexpr std::array SUPPORTED_PROPERTIES = { SHADER_TYPE_PROPERTY };

	inline const auto SHADER_PATH = fs::path(Util::RESOURCE_LOCATION).append(SHADER_SUBFOLDER);

	const Result add(const std::string& path, ShaderType type);

	const Result addBuffer(const std::string& shader, const std::string& buffer);

	const Result remove(const std::string& name);

	inline const std::string list() {
		return Util::collect(SHADER_PATH, [](fs::path path) { return path.extension() == SHADER_EXTENSION; });
	}

	inline const Result write(FILE* fp, const js::json& jsonarry) {
		return Util::writeToFile(fp, jsonarry, [=](const js::json& json, const std::string& name) {
			const ShaderType type = (ShaderType)json[SHADER_LAYOUT_PROPERTY].get<uint32_t>();
			fwrite(&type, sizeof(uint32_t), 1, fp);
			const js::json& layoutarr = json[SHADER_LAYOUT_PROPERTY];
			const size_t layoutsize = layoutarr.size();
			fwrite(&layoutsize, sizeof(uint32_t), 1, fp);
			for (const js::json& jobj : layoutarr) {

			}

			const std::string shaderPath = fs::path(name).replace_extension(SHADER_EXTENSION).string();
			size_t size;
			const uint8_t* data = Util::readFile(shaderPath, &size);
			fwrite(&size, sizeof(uint32_t), 1, fp);
			fwrite(data, sizeof(uint8_t), size, fp);
			delete[] data;
			return Result::SUCCESS;
		});
	}

}