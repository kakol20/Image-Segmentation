#include "File.h"

#include <filesystem>
#include <string>

std::string File::Extension(const std::string loc) {
	std::filesystem::path fs = loc;

	return fs.extension().string();
}