// TGTools.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <public/Texture.hpp>

using namespace std;
using namespace tgt;

const std::string subcommands[] = { "map", "actor", "texture", "font" };

typedef Result (*command)(int count, char**);
const command commandhandle = {
	[](int count, char** input) { return Result::SUCCESS; }
};

int main(int count, char** args)
{
	if (count == 0) {
		// TODO console mode
	} else {
		constexpr size_t size = sizeof(subcommands) / sizeof(subcommands[0]);
	}
	auto result = Texture::add("");
	cout << "Hello CMake. " << (int)result << endl;
	return 0;
}
