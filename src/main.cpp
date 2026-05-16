#include "image/Image.h"
#include "wrapper/Log.h"
#include <cstdlib>

int main(int argc, char* argv[]) {
	Log::WriteOneLine("Hello World!");
	
	Image img(256, 256, 3);
	img.Write("data/test.png");
	Log::Save("data/console.log");

	return EXIT_SUCCESS;
}