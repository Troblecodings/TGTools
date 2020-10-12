#include <gtest/gtest.h>
#include <filesystem>
#include <public/Model.hpp>
#include <public/Util.hpp>

using namespace tgt;

fs::path FILE_PATH;

inline std::string getFile(std::string string) {
	return fs::path(FILE_PATH).append(string).string();
}

TEST(Model, Load) {
	ASSERT_EQ(Model::loadGltf(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Model::loadGltf("doesNotExist"), Result::DOES_NOT_EXIST);
	ASSERT_EQ(Model::loadGltf(getFile("LICENSE.txt")), Result::GENERAL);

	const auto path = fs::path(getFile("glTF-Sample-Models/2.0/model-index.json"));
	ASSERT_TRUE(fs::exists(path));

	js::json model;
	JSON_LOAD(path, model);
	for (const auto& mobj : model) {
		fs::path mpath = path.parent_path();
		if (!mobj.contains("name")) {
			printf("Warning no gltf name found!\n");
			continue;
		}
		const auto namepath = fs::path(mpath).append(mobj["name"].get<std::string>());
		const auto variant = mobj["variants"];
		for (const auto& type : { "glTF", "glTF-Binary", "glTF-Embedded" }) {
			if (!variant.contains(type))
				continue;
			const auto actualpath = fs::path(namepath).append(type).append(variant[type].get<std::string>());
			ASSERT_EQ(Model::loadGltf(actualpath.string()), Result::SUCCESS);
			printf("#");
		}
	}
}

int main(int argc, char** argv) {
	fs::remove_all(Util::RESOURCE_LOCATION);
	constexpr auto stpath = "../../../../../TestFiles/";
	constexpr auto path = "TestFiles";
	if (fs::exists(stpath)) {
		FILE_PATH = stpath;
	} else if (fs::exists(path)) {
		FILE_PATH = path;
	} else {
		printf("Testfiles not found");
		return -1;
	}
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}