#include <gtest/gtest.h>
#include <public/Map.hpp>
#include <public/Util.hpp>
#include <filesystem>

using namespace tgt;

TEST(Map, Create) {
	ASSERT_EQ(Map::create("test"), Result::SUCCESS);
	ASSERT_EQ(Map::create("test"), Result::ALREADY_EXISTS);
	ASSERT_EQ(Map::create(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Map::create("../\\\\"), Result::BAD_STRING);
}

int main(int argc, char** argv) {
	fs::remove_all(Util::RESOURCE_LOCATION);
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}