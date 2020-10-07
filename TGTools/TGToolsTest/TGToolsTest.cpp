#include <gtest/gtest.h>
#include <public/Map.hpp>
#include <public/Util.hpp>
#include <public/Texture.hpp>
#include <public/Material.hpp>
#include <public/Actor.hpp>
#include <public/Font.hpp>
#include <public/Model.hpp>
#include <filesystem>

using namespace tgt;

fs::path FILE_PATH;

inline std::string getFile(std::string string) {
	return fs::path(FILE_PATH).append(string).string();
}

inline std::string trim(std::string str) {
	str.erase(std::remove_if(str.begin(), str.end(), [](char c) { return std::isspace(c); }));
	return str;
}

TEST(Texture, EmptyList) {
	ASSERT_TRUE(trim(Texture::list()).empty());
}

TEST(Texture, Add) {
	ASSERT_EQ(Texture::add(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Texture::add("thisTextureDoesNotExist"), Result::DOES_NOT_EXIST);
	ASSERT_EQ(Texture::add(getFile("test.png")), Result::SUCCESS);
	ASSERT_EQ(Texture::add(getFile("test.png")), Result::ALREADY_EXISTS);
}

TEST(Texture, List) {
	ASSERT_FALSE(trim(Texture::list()).empty());
}

TEST(Texture, Remove) {
	ASSERT_EQ(Texture::remove(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Texture::remove("test"), Result::SUCCESS);
	ASSERT_EQ(Texture::remove("test"), Result::DOES_NOT_EXIST);
	ASSERT_EQ(Texture::remove("../"), Result::BAD_STRING);
}

TEST(Material, EmptyList) {
	ASSERT_TRUE(trim(Material::list()).empty());
}

TEST(Material, Add) {
	ASSERT_EQ(Texture::add(getFile("test.png")), Result::SUCCESS);
	ASSERT_EQ(Material::add("", "test"), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Material::add("test", "thisDoesNotExist"), Result::DOES_NOT_EXIST);
	ASSERT_EQ(Material::add("test", "test"), Result::SUCCESS);
	ASSERT_EQ(Material::add("test", "test"), Result::ALREADY_EXISTS);
	ASSERT_EQ(Material::add("testcolor", "test", 0xFF000000), Result::SUCCESS);
	ASSERT_EQ(Material::add("../", "test"), Result::BAD_STRING);
	ASSERT_EQ(Material::add("..\\", "test"), Result::BAD_STRING);
}

TEST(Material, Remove) {
	ASSERT_EQ(Material::remove(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Material::remove("test"), Result::SUCCESS);
	ASSERT_EQ(Material::remove("test"), Result::DOES_NOT_EXIST);
	ASSERT_EQ(Material::remove("../"), Result::BAD_STRING);
	ASSERT_EQ(Material::remove("..\\"), Result::BAD_STRING);
}

TEST(Material, List) {
	ASSERT_FALSE(trim(Material::list()).empty());
}

TEST(Actor, EmptyList) {
	ASSERT_TRUE(trim(Actor::list()).empty());
}

TEST(Actor, Add) {
	ASSERT_EQ(Material::add("test", "test"), Result::SUCCESS);
	ASSERT_EQ(Actor::add("", "test"), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Actor::add("test", ""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Actor::add("test", "doesNotExist"), Result::DOES_NOT_EXIST);
	ASSERT_EQ(Actor::add("test", "test"), Result::SUCCESS);
	ASSERT_EQ(Actor::add("test", "test"), Result::ALREADY_EXISTS);
	ASSERT_EQ(Actor::add("../", "test"), Result::BAD_STRING);
	ASSERT_EQ(Actor::add("..\\", "test"), Result::BAD_STRING);
}

TEST(Actor, List) {
	ASSERT_FALSE(trim(Actor::list()).empty());
}

TEST(Actor, Remove) {
	ASSERT_EQ(Actor::remove(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Actor::remove("../"), Result::BAD_STRING);
	ASSERT_EQ(Actor::remove("..\\"), Result::BAD_STRING);
	ASSERT_EQ(Actor::remove("test"), Result::SUCCESS);
	ASSERT_EQ(Actor::remove("test"), Result::DOES_NOT_EXIST);
}

TEST(Font, EmptyList) {
	ASSERT_TRUE(trim(Font::list()).empty());
}

TEST(Font, Add) {
	ASSERT_EQ(Font::add(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Font::add("thisDoesNotExist"), Result::DOES_NOT_EXIST);
	ASSERT_EQ(Font::add(getFile("LICENSE.txt")), Result::GENERAL);
	ASSERT_EQ(Font::add(getFile("OpenSans-Regular.ttf")), Result::SUCCESS);
	ASSERT_TRUE(fs::exists(fs::path(Texture::TEXTURE_PATH).append("OpenSans-Regular.tgx")));
	ASSERT_EQ(Font::add(getFile("OpenSans-Regular.ttf")), Result::ALREADY_EXISTS);
}

TEST(Font, List) {
	ASSERT_FALSE(trim(Font::list()).empty());
}

TEST(Font, Remove) {
	ASSERT_EQ(Font::remove(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Font::remove("OpenSans-Regular"), Result::SUCCESS);
	ASSERT_FALSE(fs::exists(fs::path(Texture::TEXTURE_PATH).append("OpenSans-Regular.tgx")));
	ASSERT_EQ(Font::remove("OpenSans-Regular"), Result::DOES_NOT_EXIST);
	ASSERT_EQ(Font::remove("../"), Result::BAD_STRING);
	ASSERT_EQ(Font::remove("..\\"), Result::BAD_STRING);
}

TEST(Map, EmptyList) {
	ASSERT_TRUE(trim(Map::list()).empty());
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

TEST(Map, Create) {
	ASSERT_EQ(Map::create("test"), Result::SUCCESS);
	ASSERT_EQ(Map::create("test"), Result::ALREADY_EXISTS);
	ASSERT_EQ(Map::create(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Map::create("../"), Result::BAD_STRING);
	ASSERT_EQ(Map::create("..\\"), Result::BAD_STRING);
}

TEST(Map, List) {
	ASSERT_FALSE(trim(Map::list()).empty());
}

TEST(Map, Remove) {
	ASSERT_EQ(Map::remove("test"), Result::SUCCESS);
	ASSERT_EQ(Map::remove("test"), Result::DOES_NOT_EXIST);
	ASSERT_EQ(Map::remove(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Map::remove("../"), Result::BAD_STRING);
	ASSERT_EQ(Map::remove("..\\"), Result::BAD_STRING);
}

int main(int argc, char** argv) {
	fs::remove_all(Util::RESOURCE_LOCATION);
	constexpr auto stpath = "../../../../../TestFiles/";
	constexpr auto path = "/TestFiles/";
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