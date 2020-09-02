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

	constexpr auto SHADER_EXTENSION = ".glsl";
	constexpr auto SHADER_SUBFOLDER = "Shaders";

	const auto SHADER_PATH = fs::path(Util::RESOURCE_LOCATION).append(SHADER_SUBFOLDER);

	const Result add(const char* path, ShaderType type);

	const Result add(const std::string& path, ShaderType type);

	const Result remove(const char* name);

	const Result remove(const std::string& name);

	const std::string list();

}