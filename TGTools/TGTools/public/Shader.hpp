#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"
#include "../../../glslang/glslang/Public/ShaderLang.h"

namespace tgt::Shader {

	enum class ShaderType {
		VERTEX_BIT = 0x00000001,
		TESSELLATION_CONTROL_BIT = 0x00000002,
		TESSELLATION_EVALUATION_BIT = 0x00000004,
		GEOMETRY_BIT = 0x00000008,
		FRAGMENT_BIT = 0x00000010,
		COMPUTE_BIT = 0x00000020
	};

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
		SAMPLED_IMAGE = 2,
		UNIFORM_BUFFER = 6
	};

	constexpr auto DESCRIPTOR_TYPE_MIN = DescriptorType::SAMPLER;
	constexpr auto DESCRIPTOR_TYPE_MAX = DescriptorType::UNIFORM_BUFFER;

	constexpr auto SHADER_EXTENSION = ".glsl";
	constexpr auto SHADER_SUBFOLDER = "Shaders";
	
	constexpr auto SHADER_TYPE_PROPERTY = "shaderType";
	constexpr auto SHADER_LAYOUT_PROPERTY = "shaderLayouts";

	constexpr auto SHADER_LAYOUT_TYPE_PROPERTY = "shaderLayoutType";
	constexpr auto SHADER_LAYOUT_BINDING_PROPERTY = "shaderLayoutBinding";
	constexpr auto SHADER_LAYOUT_RESOURCE_PROPERTY = "shaderLayoutResource";

	constexpr std::array SUPPORTED_PROPERTIES = { SHADER_TYPE_PROPERTY };

	inline const auto SHADER_PATH = fs::path(Util::RESOURCE_LOCATION).append(SHADER_SUBFOLDER);

	const Result add(const std::string& path, const ShaderType type);

	const Result addStaticInput(const std::string& shader, const std::string& resource, const uint32_t binding, const DescriptorType type);

	const Result remove(const std::string& name);

	inline const std::string list() {
		return Util::collect(SHADER_PATH, [](fs::path path) { return path.extension() == SHADER_EXTENSION; });
	}

	const Result write(FILE* fp, const js::json& jsonarry);

}