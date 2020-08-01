// TGTools.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <public/Texture.hpp>

using namespace std;
using namespace tgt;

const std::string subcommands[] = { "map", "actor", "texture", "font" };

typedef Result (*command)(int count, char**);
constexpr command commandhandle[] = {
	[](int count, char** input) { return Result::GENERAL; },
	[](int count, char** input) { return Result::GENERAL; },
	[](int count, char** input) { return Result::GENERAL; },
	[](int count, char** input) { return Result::GENERAL; }
};

constexpr auto sizecommand = sizeof(subcommands) / sizeof(subcommands[0]);
constexpr auto sizehandles = sizeof(commandhandle) / sizeof(commandhandle[0]);

// Compile time check for dumb people like me
static_assert(sizehandles == sizecommand, "Subcommand size and handler size are not equal!");

static Result exec(int count, char** args) {
	args[]
}

int main(int count, char** args)
{
	if (count == 0) {
		// TODO console mode
	} else {
		return (int)exec(count, args);
	}
	auto result = Texture::add("");
	cout << "Hello CMake. " << (int)result << endl;
	return 0;
}
