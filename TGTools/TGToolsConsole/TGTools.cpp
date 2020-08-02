// TGTools.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <public/Texture.hpp>
#include <public/Actor.hpp>
#include <public/Map.hpp>
#include <public/Font.hpp>
#include <map>
#include <vector>
#include <sstream>

using namespace std;
using namespace tgt;

typedef Result(*commandhandle)(const char** args);

struct Command{
	size_t count;
	commandhandle handle;
};

const std::map<std::string, std::map<std::string, Command>> commands =
{
	{ 
		"actor", {
			{ "add", { 2, [](auto args) { return Actor::add(args[0], args[1]); } } },
			{ "remove", { 1, [](auto args) { return Actor::remove(args[0]); } } },
			{ "list", { 0, [](auto args) { std::cout << Actor::list() << std::endl; return Result::SUCCESS; } } }
		}
	},
	{
		"map", {
			{ "create", {1, [](auto args) { return Map::create(args[0]); } } },
			{ "delete", {1, [](auto args) { return Map::remove(args[0]); } } },
			{ "list", { 0, [](auto args) { std::cout << Map::list() << std::endl; return Result::SUCCESS; } } },
			{ "add", {2, [](auto args) { return Map::add(args[0], args[1]); } } },
			{ "remove", {2, [](auto args) { return Map::remove(args[0], args[1]); } } },
			{ "make", {1, [](auto args) { return Map::make(args[0]); } } }
		}
	}, 
	{
		"texture", {
			{ "add", {1, [](auto args) { return Texture::add(args[0]); } } },
			{ "remove", {1, [](auto args) { return Texture::remove(args[0]); } } },
			{ "list", { 0, [](auto args) { std::cout << Texture::list() << std::endl; return Result::SUCCESS; } } }
		}
	},
	{
		"font", {
			{ "add", {1, [](auto args) { return Font::add(args[0]); } } },
			{ "remove", {1, [](auto args) { return Font::remove(args[0]); } } },
			{ "list", { 0, [](auto args) { std::cout << Font::list() << std::endl; return Result::SUCCESS; } } }
		}
	}
};

static Result exec(int count, const char** args) {
	if (count < 2) return Result::BAD_ARGUMENTS;

	const std::string subcommand = args[0];
	const auto listitr = commands.find(subcommand);
	if (listitr == commands.end()) return Result::BAD_ARGUMENTS;
	const auto& list = listitr->second;

	const std::string subsubcommand = args[1];
	const auto commanditr = list.find(subsubcommand);
	if (commanditr == list.end()) return Result::BAD_ARGUMENTS;
	const auto& command = commanditr->second;

	// Forward ptr
	count -= 2;
	args += 2;

	if (count < command.count) return Result::BAD_ARGUMENTS;
	return command.handle(args);
}

int main(int count, const char** args) {
	count--;
	args++;
	if (count == 0) {
		while (true) {
			std::cout << ">>>";

			std::string text;
			getline(cin, text);
			if (text == "exit")
				break;

			std::istringstream iss(text);
			std::vector<std::string> results(std::istream_iterator<std::string>{iss},
				std::istream_iterator<std::string>());

			const int count = results.size();
			args = new const char* [count];
			for (size_t i = 0; i < count; i++)
				args[i] = results[i].c_str();

			const Result result = exec(count, args);
			if (result != Result::SUCCESS)
				std::cout << "Error " << (int)result << std::endl;

			delete[] args;
		}
		return 0;
	} else {
		return (int)exec(count, args);
	}
}
