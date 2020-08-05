#include "../public/Font.hpp"
#include "../public/json.hpp"
#include "../public/Texture.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../stb/stb_rect_pack.h"
#include "../../../stb/stb_truetype.h"
#include "../../../stb/stb_image_write.h"
#include <iostream>

namespace tgt::Font {

	namespace js = nlohmann;

	const Result add(const char* path) {
		STRING_CHECKS_C(path);
		return add(std::string(path));
	}

	static const Result generatefontbitmap(const fs::path& resourceLocation) {
		constexpr auto START_CHAR = 33, END_CHAR = 126, DIFF = END_CHAR - START_CHAR;
		constexpr auto STRIDE = 1, WIDTH = 64, HEIGHT = 64;

		constexpr auto DIFF_HALF = (int)(((float)DIFF) / 4.0f) + 1;
		constexpr auto PHEIGHT = HEIGHT * DIFF_HALF, PWIDTH = WIDTH * DIFF_HALF;

		constexpr auto BUFFERSIZE = STRIDE * PHEIGHT * PWIDTH;


		uint8_t* buffer = new uint8_t[BUFFERSIZE];

		Util::scope_exit exit([=]() { delete[] buffer; });

		stbtt_pack_context packedcontext;
		if (!stbtt_PackBegin(&packedcontext, buffer, PWIDTH, PHEIGHT, STRIDE, 0, 0))
			return Result::GENERAL;

		std::ifstream input(resourceLocation, std::iostream::binary | std::iostream::ate | std::iostream::in);
		auto size = (size_t)input.tellg();
		uint8_t* fontData = new uint8_t[size];

		Util::scope_exit fexit([=]() { delete[] fontData; });

		input.seekg(0, std::iostream::_Seekbeg);
		input.read((char*)fontData, size);

		stbtt_PackSetOversampling(&packedcontext, 1, 1);

		stbtt_packedchar* packedchars = new stbtt_packedchar[DIFF];
		if (!stbtt_PackFontRange(&packedcontext, fontData, 0, HEIGHT, START_CHAR, DIFF, packedchars))
			return Result::GENERAL;

		stbtt_PackEnd(&packedcontext);

		const auto texturelocation = Util::getResource(Texture::TEXTURE_PATH, resourceLocation.stem().string(), Texture::TEXTURE_EXTENSION).string();

		if(!stbi_write_png(texturelocation.c_str(), PWIDTH, PHEIGHT, STRIDE, buffer, 1))
			return Result::GENERAL;

		return Result::SUCCESS;
	}

	const Result add(const std::string& path) {
		STRING_CHECKS(path);
		const fs::path fontPath(path);

		if (!fs::exists(fontPath))
			return Result::DOES_NOT_EXIST;

		auto resourceLocation = Util::getResource(FONT_PATH, fontPath.filename().string());
		if (!fs::copy_file(fontPath, resourceLocation)) // Failing if destination exists
			return Result::ALREADY_EXISTS;

		auto result = generatefontbitmap(resourceLocation);
		if (result != Result::SUCCESS) {
			fs::remove(resourceLocation);
			return result;
		}
		return Result::SUCCESS;
	}

	const Result remove(const char* name) {
		STRING_CHECKS_C(name);
		return remove(std::string(name));
	}

	const Result remove(const std::string& name) {
		STRING_CHECKS(name);

		auto font = Util::getResource(FONT_PATH, name, FONT_EXTENSION);

		if (!fs::remove(font))
			return Result::DOES_NOT_EXIST;

		return Result::SUCCESS;
	}

	const std::string list() {
		return Util::collect(FONT_PATH, [](fs::path path) { return path.extension() == FONT_EXTENSION; });
	}
}