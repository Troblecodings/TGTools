#pragma once

#include "Result.hpp"
#include <string>
#include "Util.hpp"
#include <glslang/Public/ShaderLang.h>

namespace tgt::Shader {

	using ShaderType = EShLanguage;

	constexpr auto SHADER_EXTENSION = ".glsl";
	constexpr auto SHADER_SUBFOLDER = "Shaders";

	const auto SHADER_PATH = fs::path(Util::RESOURCE_LOCATION).append(SHADER_SUBFOLDER);

	const Result add(const char* path, ShaderType type);

	const Result add(const std::string& path, ShaderType type);

	const Result remove(const char* name);

	const Result remove(const std::string& name);

	const std::string list();

}