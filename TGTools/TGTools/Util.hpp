#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace tgt::Util {

	template<class T>
	const fs::path getResource(T name) {
		fs::path resource("./Resource/");
		if (!fs::exists(resource))
			fs::create_directories(resource);
		return resource.append(name);
	}

}