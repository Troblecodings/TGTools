#include <gtest/gtest.h>
#include <public/Map.hpp>
#include <public/Util.hpp>
#include <public/Texture.hpp>
#include <public/Material.hpp>
#include <public/Actor.hpp>
#include <public/Font.hpp>
#include <filesystem>

using namespace tgt;

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
	ASSERT_EQ(Texture::add("TestFiles/test.png"), Result::SUCCESS);
	ASSERT_EQ(Texture::add("TestFiles/test.png"), Result::ALREADY_EXISTS);
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
	ASSERT_EQ(Texture::add("TestFiles/test.png"), Result::SUCCESS);
	ASSERT_EQ(Material::add("test", ""), Result::BAD_ARGUMENTS);
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
	ASSERT_EQ(Font::add("TestFiles/LICENSE.txt"), Result::GENERAL);
	ASSERT_EQ(Font::add("TestFiles/OpenSans-Regular.ttf"), Result::SUCCESS);
	ASSERT_TRUE(fs::exists(fs::path(Texture::TEXTURE_PATH).append("OpenSans-Regular.tgx")));
	ASSERT_EQ(Font::add("TestFiles/OpenSans-Regular.ttf"), Result::ALREADY_EXISTS);
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
	constexpr auto file = "../../../../../TestFiles/";
	constexpr auto destination = "TestFiles";
	if (fs::exists(file)) {
		fs::remove_all(destination);
		fs::copy(file, destination);
	}
	if (!fs::exists(destination)) {
		printf("Testfiles not found");
		return -1;
	}
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}