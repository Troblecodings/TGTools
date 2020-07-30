// TGTools.cpp : Defines the entry point for the application.
//

#include "TGTools.h"
#include <Texture.hpp>

using namespace std;
using namespace tgt;

int main()
{
	auto result = Texture::add("");
	cout << "Hello CMake. " << (int)result << endl;
	return 0;
}
