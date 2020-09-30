#include "../public/Font.hpp"
#include "../public/json.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../stb/stb_rect_pack.h"
#include "../../../stb/stb_truetype.h"
#include "../../../stb/stb_image_write.h"
#include <iostream>

namespace tgt::Font {

	constexpr auto TGF_HEADER_VERSION = 1;

	struct TGFHeader {
		uint32_t version = TGF_HEADER_VERSION;
		uint32_t textureindex = 0; //reserved
		uint32_t startindex;
		uint32_t count;
	};

	static const Result generatefontbitmap(const fs::path& font, const std::string& resourceLocation, const std::string& texture) {
		constexpr auto START_CHAR = 33, END_CHAR = 126, DIFF = END_CHAR - START_CHAR;
		constexpr auto STRIDE = 1, WIDTH = 64, HEIGHT = 64;

		constexpr auto DIFF_HALF = (int)(((float)DIFF) / 4.0f) + 1;
		constexpr auto PHEIGHT = HEIGHT * DIFF_HALF, PWIDTH = WIDTH * DIFF_HALF;

		constexpr auto BUFFERSIZE = STRIDE * PHEIGHT * PWIDTH;

		uint8_t* buffer = new uint8_t[BUFFERSIZE];

		Util::scope_exit exit([=]() { delete[] buffer; });

		stbtt_pack_context packedcontext;
		if (!stbtt_PackBegin(&packedcontext, buffer, PWIDTH, PHEIGHT, 0, STRIDE, 0))
			return Result::GENERAL;

		auto fontData = Util::readFile(font.string());
		Util::scope_exit fexit([=]() { delete[] fontData; });

		stbtt_PackSetOversampling(&packedcontext, 2, 2);

		stbtt_packedchar* packedchars = new stbtt_packedchar[DIFF];
		if (!stbtt_PackFontRange(&packedcontext, fontData, 0, HEIGHT, START_CHAR, DIFF, packedchars)) {
			delete[] packedchars;
			return Result::GENERAL;
		}

		stbtt_PackEnd(&packedcontext);


		uint8_t* finalbuffer = new uint8_t[BUFFERSIZE * 4];

		Util::scope_exit bexit([=]() { delete[] finalbuffer; });

		for (size_t x = 0; x < BUFFERSIZE; x++) {
			auto value = buffer[x];
			auto index = x * 4;
			finalbuffer[index] = value;
			finalbuffer[index + 1] = value;
			finalbuffer[index + 2] = value;
			finalbuffer[index + 3] = value;
		}

		if(!stbi_write_png(texture.c_str(), PWIDTH, PHEIGHT, 4, finalbuffer, PWIDTH * 4))
			return Result::GENERAL;

		FILE* file = fopen(resourceLocation.c_str(), "wb");
		if (!file)
			return Result::GENERAL;
		TGFHeader header;
		header.startindex = START_CHAR;
		header.count = DIFF;

		fwrite(&header, sizeof(TGFHeader), 1, file);
		fwrite(packedchars, sizeof(stbtt_packedchar), DIFF, file);
		fclose(file);
		return Result::SUCCESS;
	}

	const Result add(const std::string& path) {
		STRING_CHECKS(path);
		const fs::path fontPath(path);

		if (!fs::exists(fontPath))
			return Result::DOES_NOT_EXIST;

		const auto fontName = fontPath.stem().string();
		const auto resourceLocation = Util::getResource(FONT_PATH, fontName, FONT_EXTENSION).string();
		const auto texturelocation = Util::getResource(Texture::TEXTURE_PATH, fontName, Texture::TEXTURE_EXTENSION).string();

		auto result = generatefontbitmap(fontPath, resourceLocation, texturelocation);
		if (result != Result::SUCCESS) {
			fs::remove(resourceLocation);
			fs::remove(texturelocation);
			return result;
		}
		return Result::SUCCESS;
	}

}