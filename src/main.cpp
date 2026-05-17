#include "image/Image.h"
#include "wrapper/File.h"
#include "wrapper/Log.h"
#include <cstdlib>
#include <string>

int main(int argc, char* argv[]) {
	Log::WriteOneLine("argc: " + Log::ToString(argc));

	for (int i = 0; i < argc; ++i) {
		//Log::WriteOneLine("  " + static_cast<std::string>(argv[i]));

		if (File::Extension(argv[i]) == ".exe") continue;
		if (File::Extension(argv[i]) == ".json") {
			Log::WriteOneLine("  JSON: " + static_cast<std::string>(argv[i]));
			continue;
		}

		if (Image::ValidFileType(argv[i])) {
			Log::WriteOneLine("  Image: " + static_cast<std::string>(argv[i]));
			continue;
		}

		Log::WriteOneLine("  Other: " + static_cast<std::string>(argv[i]));
	}

	//Log::HoldConsole();
	Log::Save("console.log");

	return EXIT_SUCCESS;
}