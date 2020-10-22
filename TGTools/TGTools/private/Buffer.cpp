#include "../public/Buffer.hpp"

namespace tgt::Buffer {

	const Result add(const std::string& name, const BufferTypeFlags type, const size_t size) {
		STRING_CHECKS(name);
		STRING_SYNTAX_CHECK(name);
		ENUM_CHECKS(type, BUFFER_TYPE_MIN, BUFFER_TYPE_MAX);

		if (size == 0)
			return Result::BAD_ARGUMENTS;

		const auto bufferpath = Util::getResource(BUFFER_PATH, name, Util::JSON);
		if (fs::exists(bufferpath))
			return Result::ALREADY_EXISTS;

		js::json json;
		json[TYPE_PROPERTY] = (size_t)type;
		json[SIZE_PROPERTY] = size;
		JSON_WRITE(bufferpath, json);

		return Result::SUCCESS;
	}

}