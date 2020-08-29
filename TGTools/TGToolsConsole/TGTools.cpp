// TGTools.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <public/Texture.hpp>
#include <public/Actor.hpp>
#include <public/Map.hpp>
#include <public/Font.hpp>
#include <public/Shader.hpp>
#include <map>
#include <vector>
#include <sstream>
#include <exception>
#include <iterator>

using namespace std;
using namespace tgt;

typedef Result(*commandhandle)(int count, const char** args);

struct Command {
	size_t count;
	commandhandle handle;
};

static Result actorchange(int count, const char** inputs) {
	const std::string value = inputs[2];
	try {
		if (value.find('.') == UINT64_MAX) {
			auto convertint = stoi(value);
			return Actor::change(inputs[0], inputs[1], convertint);
		} else {
			auto convertfloat = stof(value);
			return Actor::change(inputs[0], inputs[1], convertfloat);
		}
	} catch (const std::exception&) {
		return Actor::change(inputs[0], inputs[1], value);
	}
}

const std::map<std::string, std::map<std::string, Command>> commands =
{
	{
		"actor", {
			{ "add", { 2, [](int count, auto args) { return Actor::add(args[0], args[1]); } } },
			{ "remove", { 1, [](int count, auto args) { return Actor::remove(args[0]); } } },
			{ "list", { 0, [](int count, auto args) { std::cout << Actor::list() << std::endl; return Result::SUCCESS; } } },
			{ "change", { 3, actorchange } }
		}
	},
	{
		"map", {
			{ "create", {1, [](int count, auto args) { return Map::create(args[0]); } } },
			{ "delete", {1, [](int count, auto args) { return Map::remove(args[0]); } } },
			{ "list", { 0, [](int count, auto args) { std::cout << Map::list() << std::endl; return Result::SUCCESS; } } },
			{ "add", {2, [](int count, auto args) { return Map::add(args[0], args[1]); } } },
			{ "remove", {2, [](int count, auto args) { return Map::remove(args[0], args[1]); } } },
			{ "make", {1, [](int count, auto args) { return Map::make(args[0]); } } }
		}
	},
	{
		"texture", {
			{ "add", {1, [](int count, auto args) { return Texture::add(args[0]); } } },
			{ "remove", {1, [](int count, auto args) { return Texture::remove(args[0]); } } },
			{ "list", { 0, [](int count, auto args) { std::cout << Texture::list() << std::endl; return Result::SUCCESS; } } }
		}
	},
	{
		"font", {
			{ "add", {1, [](int count, auto args) { return Font::add(args[0]); } } },
			{ "remove", {1, [](int count, auto args) { return Font::remove(args[0]); } } },
			{ "list", { 0, [](int count, auto args) { std::cout << Font::list() << std::endl; return Result::SUCCESS; } } }
		}
	},
	{
		"shader", {
			{"add", {2, [](int count, auto args) {return Shader::add(args[0], (Shader::ShaderType) std::stoi(args[1])); }}}
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
	return command.handle(count, args);
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

			const size_t count = results.size();
			args = new const char* [count];
			for (size_t i = 0; i < count; i++)
				args[i] = results[i].c_str();

			const Result result = exec((int)count, args);
			if (result != Result::SUCCESS)
				std::cout << "Error (" << (int)result << ")!" << std::endl;

			delete[] args;
		}
		return 0;
	} else {
		return (int)exec(count, args);
	}
}
