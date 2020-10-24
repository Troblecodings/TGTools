#pragma once

#include "Util.hpp"

namespace tgt::Buffer {

    /*
     * This indicates the buffer usage
     * One buffer can be used for more purposes simultaniously
     * It therfore can be a bit mask
     */
	enum BufferTypeFlags {
        TRANSFER_SRC_BIT = 0x00000001,
        TRANSFER_DST_BIT = 0x00000002,
        UNIFORM_TEXEL_BUFFER_BIT = 0x00000004,
        STORAGE_TEXEL_BUFFER_BIT = 0x00000008,
        UNIFORM_BUFFER_BIT = 0x00000010,
        STORAGE_BUFFER_BIT = 0x00000020,
        INDEX_BUFFER_BIT = 0x00000040,
        VERTEX_BUFFER_BIT = 0x00000080,
        INDIRECT_BUFFER_BIT = 0x00000100,
	};

    constexpr size_t allflagsenabled() {
        return TRANSFER_SRC_BIT |
            TRANSFER_DST_BIT |
            UNIFORM_TEXEL_BUFFER_BIT |
            STORAGE_TEXEL_BUFFER_BIT |
            UNIFORM_BUFFER_BIT |
            STORAGE_BUFFER_BIT |
            INDEX_BUFFER_BIT |
            VERTEX_BUFFER_BIT |
            INDIRECT_BUFFER_BIT;
    }

    constexpr auto BUFFER_TYPE_MAX = allflagsenabled();
    constexpr auto BUFFER_TYPE_MIN = TRANSFER_SRC_BIT;

    constexpr auto BUFFER_SUBFOLDER = "Buffer";
    constexpr auto BUFFER_EXTENSION = ".tgrb";

    constexpr auto TYPE_PROPERTY = "type";
    constexpr auto SIZE_PROPERTY = "size";

    const auto BUFFER_PATH = fs::path(Util::RESOURCE_LOCATION).append(BUFFER_SUBFOLDER);

	const Result add(const std::string& name, const BufferTypeFlags type, const size_t size);

    inline const Result remove(const std::string& name) {
        STRING_CHECKS(name);
        STRING_SYNTAX_CHECK(name);

        auto result = Util::remove(BUFFER_PATH, name);
        if (result == Result::DEPENDENT)
            return result;
        if(Util::remove(BUFFER_PATH, name, BUFFER_EXTENSION) == Result::DOES_NOT_EXIST)
            return result;
    }

    inline const std::string list() {
        return Util::collect(BUFFER_PATH, Util::JSON_FILTER);
    }

    inline const Result write(FILE* file, const js::json& array) {
        return Util::writeToFile(file, array, [=](const js::json& json, const std::string& name) {
            fwrite(&json[TYPE_PROPERTY], sizeof(size_t), 1, file);
            fwrite(&json[SIZE_PROPERTY], sizeof(size_t), 1, file);

            const auto contentpath = fs::path(name).replace_extension(BUFFER_EXTENSION);
            if (!fs::exists(contentpath)) {
                constexpr size_t zero = 0;
                fwrite(&zero, sizeof(size_t), 1, file);
            } else {
                size_t size = 0;
                const uint8_t* ptr = Util::readFile(contentpath.string(), &size);
                fwrite(&size, sizeof(size_t), 1, file);
                fwrite(ptr, sizeof(uint8_t), size, file);
                delete[] ptr;
            }
            return Result::SUCCESS;
        });
    }
}