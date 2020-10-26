#include <gtest/gtest.h>
#include <public/Map.hpp>
#include <public/Util.hpp>
#include <public/Texture.hpp>
#include <public/Material.hpp>
#include <public/Actor.hpp>
#include <public/Font.hpp>
#include <public/Model.hpp>
#include <public/Sampler.hpp>
#include <public/Buffer.hpp>
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

TEST(Actor, Data) {
	float testVertices[12] = {
		0, 0, 0, 0,
		0, 1, 0, 1,
		1, 0, 1, 0
	};

	uint32_t testIndices[3] = { 0, 1, 2 };

	ASSERT_EQ(Actor::setData("", Actor::ActorDataType::VERTEX, testVertices, sizeof(testVertices)), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Actor::setData("../", Actor::ActorDataType::VERTEX, testVertices, sizeof(testVertices)), Result::BAD_STRING);
	ASSERT_EQ(Actor::setData("..\\", Actor::ActorDataType::VERTEX, testVertices, sizeof(testVertices)), Result::BAD_STRING);
	ASSERT_EQ(Actor::setData("test", (Actor::ActorDataType)3, testVertices, sizeof(testVertices)), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Actor::setData("test", Actor::ActorDataType::VERTEX, testVertices, 0), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Actor::setData("test", Actor::ActorDataType::VERTEX, nullptr, sizeof(testVertices)), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Actor::setData("doesNotExist", Actor::ActorDataType::VERTEX, testVertices, sizeof(testVertices)), Result::DOES_NOT_EXIST);

	ASSERT_EQ(Actor::setData("test", Actor::ActorDataType::VERTEX, testVertices, sizeof(testVertices), true), Result::SUCCESS);
	ASSERT_EQ(Actor::setData("test", Actor::ActorDataType::VERTEX, testVertices, sizeof(testVertices)), Result::SUCCESS);
	ASSERT_EQ(Actor::setData("test", Actor::ActorDataType::VERTEX, testVertices, sizeof(testVertices), true), Result::SUCCESS);

	ASSERT_EQ(Actor::setData("test", Actor::ActorDataType::INDEX, testIndices, sizeof(testIndices), true), Result::SUCCESS);
	ASSERT_EQ(Actor::setData("test", Actor::ActorDataType::INDEX, testIndices, sizeof(testIndices)), Result::SUCCESS);
	ASSERT_EQ(Actor::setData("test", Actor::ActorDataType::INDEX, testIndices, sizeof(testIndices), true), Result::SUCCESS);

	float* testVerticesRead;
	const void** textVerticeDataRead = (const void**)&testVerticesRead;
	size_t size;
	ASSERT_EQ(Actor::getData("", Actor::ActorDataType::VERTEX, textVerticeDataRead), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Actor::getData("../", Actor::ActorDataType::VERTEX, textVerticeDataRead), Result::BAD_STRING);
	ASSERT_EQ(Actor::getData("..\\", Actor::ActorDataType::VERTEX, textVerticeDataRead), Result::BAD_STRING);
	ASSERT_EQ(Actor::getData("test", (Actor::ActorDataType)3, textVerticeDataRead), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Actor::getData("test", Actor::ActorDataType::VERTEX, nullptr), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Actor::getData("doesNotExist", Actor::ActorDataType::VERTEX, textVerticeDataRead, &size), Result::DOES_NOT_EXIST);

	ASSERT_EQ(Actor::getData("test", Actor::ActorDataType::VERTEX, textVerticeDataRead, &size), Result::SUCCESS);
	ASSERT_EQ(size, sizeof(testVertices) * 2);
	ASSERT_NE(testVerticesRead, nullptr);
	delete[] testVerticesRead;
	ASSERT_EQ(Actor::getData("test", Actor::ActorDataType::VERTEX, textVerticeDataRead), Result::SUCCESS);
	ASSERT_NE(testVerticesRead, nullptr);

	uint32_t* testIndicesRead;
	const void** textIndicesDataRead = (const void**)&testIndicesRead;
	size_t sizeIndices;
	ASSERT_EQ(Actor::getData("test", Actor::ActorDataType::INDEX, textIndicesDataRead, &sizeIndices), Result::SUCCESS);
	ASSERT_EQ(sizeIndices, sizeof(testIndices) * 2);
	ASSERT_NE(testIndicesRead, nullptr);
	delete[] testIndicesRead;
	ASSERT_EQ(Actor::getData("test", Actor::ActorDataType::INDEX, textIndicesDataRead), Result::SUCCESS);
	ASSERT_NE(testIndicesRead, nullptr);

	constexpr auto indexElements = sizeof(testIndices) / sizeof(*testIndices);
	for (size_t x = 0; x < 2; x++)
		for (size_t i = 0; i < indexElements; i++)
			ASSERT_EQ(testIndices[i], testIndicesRead[i + x * indexElements]);

	constexpr auto vertexElements = sizeof(testVertices) / sizeof(*testVertices);
	for (size_t x = 0; x < 2; x++)
		for (size_t i = 0; i < vertexElements; i++)
			ASSERT_FLOAT_EQ(testVertices[i], testVerticesRead[i + x * vertexElements]);

	delete[] testVerticesRead;
	delete[] testIndicesRead;
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

TEST(Sampler, EmptyList) {
	ASSERT_TRUE(trim(Sampler::list()).empty());
}

TEST(Sampler, Add) {
	ASSERT_EQ(Sampler::add("", Sampler::SamplerAddressMode::REPEAT, Sampler::SamplerFilter::LINEAR), Result::BAD_ARGUMENTS);

	ASSERT_EQ(Sampler::add("test", (Sampler::SamplerAddressMode)0xFFFF, Sampler::SamplerAddressMode::REPEAT,
		Sampler::SamplerFilter::LINEAR, Sampler::SamplerFilter::LINEAR), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Sampler::add("test", Sampler::SamplerAddressMode::REPEAT, (Sampler::SamplerAddressMode)0xFFFF,
		Sampler::SamplerFilter::LINEAR, Sampler::SamplerFilter::LINEAR), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Sampler::add("test", Sampler::SamplerAddressMode::REPEAT, Sampler::SamplerAddressMode::REPEAT,
		Sampler::SamplerFilter::LINEAR, (Sampler::SamplerFilter) 0xFFFF), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Sampler::add("test", Sampler::SamplerAddressMode::REPEAT, Sampler::SamplerAddressMode::REPEAT,
		(Sampler::SamplerFilter) 0xFFFF, Sampler::SamplerFilter::LINEAR), Result::BAD_ARGUMENTS);

	ASSERT_EQ(Sampler::add("test", Sampler::SamplerAddressMode::REPEAT, Sampler::SamplerFilter::LINEAR), Result::SUCCESS);
	ASSERT_EQ(Sampler::add("test", Sampler::SamplerAddressMode::REPEAT, Sampler::SamplerFilter::LINEAR), Result::ALREADY_EXISTS);

	ASSERT_EQ(Sampler::add("../", Sampler::SamplerAddressMode::REPEAT, Sampler::SamplerFilter::LINEAR), Result::BAD_STRING);
	ASSERT_EQ(Sampler::add("..\\", Sampler::SamplerAddressMode::REPEAT, Sampler::SamplerFilter::LINEAR), Result::BAD_STRING);
}

TEST(Sampler, List) {
	ASSERT_FALSE(trim(Sampler::list()).empty());
}

TEST(Sampler, Change) {
	ASSERT_EQ(Sampler::change("test", "doesNotExist", Sampler::SamplerFilter::LINEAR), Result::UNSUPPORTED);
	ASSERT_EQ(Sampler::change("", Sampler::MAG_FILTER_PROPERTY, Sampler::SamplerFilter::LINEAR), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Sampler::change("test", Sampler::MAG_FILTER_PROPERTY, Sampler::SamplerFilter::LINEAR), Result::SUCCESS);
	ASSERT_EQ(Sampler::change("test", Sampler::MIN_FILTER_PROPERTY, Sampler::SamplerFilter::LINEAR), Result::SUCCESS);
	ASSERT_EQ(Sampler::change("test", Sampler::UMODE_PROPERTY, Sampler::SamplerAddressMode::REPEAT), Result::SUCCESS);
	ASSERT_EQ(Sampler::change("test", Sampler::VMODE_PROPERTY, Sampler::SamplerAddressMode::REPEAT), Result::SUCCESS);
}

TEST(Sampler, Remove) {
	ASSERT_EQ(Sampler::remove(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Sampler::remove("test"), Result::SUCCESS);
	ASSERT_EQ(Sampler::remove("test"), Result::DOES_NOT_EXIST);
	ASSERT_EQ(Sampler::remove("../"), Result::BAD_STRING);
	ASSERT_EQ(Sampler::remove("..\\"), Result::BAD_STRING);
}

TEST(Buffer, EmptyList) {
	ASSERT_TRUE(trim(Buffer::list()).empty());
}

TEST(Buffer, Add) {
	ASSERT_EQ(Buffer::add("", Buffer::BufferTypeFlags::VERTEX_BUFFER_BIT, 100), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Buffer::add("", Buffer::BufferTypeFlags::VERTEX_BUFFER_BIT, 0), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Buffer::add("test", (Buffer::BufferTypeFlags)0, 100), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Buffer::add("test", (Buffer::BufferTypeFlags)(Buffer::allflagsenabled() + 1), 100), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Buffer::add("test", (Buffer::BufferTypeFlags)Buffer::allflagsenabled(), 100), Result::SUCCESS);
	ASSERT_EQ(Buffer::add("test", (Buffer::BufferTypeFlags)Buffer::allflagsenabled(), 100), Result::ALREADY_EXISTS);
	ASSERT_EQ(Buffer::add("../", (Buffer::BufferTypeFlags)Buffer::allflagsenabled(), 100), Result::BAD_STRING);
	ASSERT_EQ(Buffer::add("..\\", (Buffer::BufferTypeFlags)Buffer::allflagsenabled(), 100), Result::BAD_STRING);
}

TEST(Buffer, List) {
	ASSERT_FALSE(trim(Buffer::list()).empty());
}

TEST(Buffer, Remove) {
	ASSERT_EQ(Buffer::remove(""), Result::BAD_ARGUMENTS);
	ASSERT_EQ(Buffer::remove("../"), Result::BAD_STRING);
	ASSERT_EQ(Buffer::remove("..\\"), Result::BAD_STRING);
	ASSERT_EQ(Buffer::remove("test"), Result::SUCCESS);
	ASSERT_EQ(Buffer::remove("test"), Result::DOES_NOT_EXIST);
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