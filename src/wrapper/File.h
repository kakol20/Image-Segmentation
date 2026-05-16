#pragma once
#include <string>

class File {
public:
	File() {};
	~File() {};

	static std::string Extension(const std::string loc);
};