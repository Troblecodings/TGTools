#pragma once

enum class Result {
	SUCCESS,
	BAD_ARGUMENTS,
	ALREADY_EXISTS,
	DOES_NOT_EXIST,
	DEPENDENT,
	GENERAL,
	BAD_STRING,
	UNSUPPORTED,
	WARNINGS
};