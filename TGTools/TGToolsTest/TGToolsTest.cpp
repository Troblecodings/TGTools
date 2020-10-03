#include <gtest/gtest.h>
#include <public/Map.hpp>
#include <public/Util.hpp>
#include <public/Texture.hpp>
#include <public/Material.hpp>
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
	ASSERT_EQ(Texture::add("../../../../TestFiles/test.png"), Result::SUCCESS);
	ASSERT_EQ(Texture::add("../../../../TestFiles/test.png"), Result::ALREADY_EXISTS);
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
	ASSERT_EQ(Texture::add("../../../../TestFiles/test.png"), Result::SUCCESS);
	ASSERT_EQ(Material::add("test", ""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Material::add("", "test"), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Material::add("test", "test"), Result::SUCCESS);
	ASSERT_EQ(Material::add("test", "test"), Result::ALREADY_EXISTS);
	ASSERT_EQ(Material::add("testcolor", "test", 0xFF000000), Result::SUCCESS);
}

TEST(Material, Remove) {
	ASSERT_EQ(Material::remove(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Material::remove("test"), Result::SUCCESS);
	ASSERT_EQ(Material::remove("test"), Result::DOES_NOT_EXIST);
	ASSERT_EQ(Material::remove("../"), Result::BAD_STRING);
}

TEST(Material, List) {
	ASSERT_FALSE(trim(Material::list()).empty());
}

TEST(Map, EmptyList) {
	ASSERT_TRUE(trim(Map::list()).empty());
}

TEST(Map, Create) {
	ASSERT_EQ(Map::create("test"), Result::SUCCESS);
	ASSERT_EQ(Map::create("test"), Result::ALREADY_EXISTS);
	ASSERT_EQ(Map::create(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Map::create("../"), Result::BAD_STRING);
}

TEST(Map, List) {
	ASSERT_FALSE(trim(Map::list()).empty());
}

TEST(Map, Remove) {
	ASSERT_EQ(Map::remove("test"), Result::SUCCESS);
	ASSERT_EQ(Map::remove("test"), Result::DOES_NOT_EXIST);
	ASSERT_EQ(Map::remove(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Map::remove("../"), Result::BAD_STRING);
}

int main(int argc, char** argv) {
	fs::remove_all(Util::RESOURCE_LOCATION);
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}