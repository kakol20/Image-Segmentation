#include "File.h"

#include <iostream>
#include <string>
#include <filesystem>

std::string File::Extension(const std::string loc) {
	std::filesystem::path fs = loc;

	return fs.extension().string();
}