#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "../ext/json/json.hpp"
#include "clustering/Colour.h"
#include "clustering/KMeans.h"
#include "clustering/MeanShift.h"
#include "colourSpace/ColorSpace.hpp"
#include "colourSpace/OkLab.h"
#include "colourSpace/OkLCh.h"
#include "colourSpace/sRGB.hpp"
#include "image/Image.h"
#include "maths/Maths.hpp"
#include "other/Log.h"
#include "other/Random.h"

//#define DEBUG

using json = nlohmann::json;

const double Maths::Pi = 3.1415926535;
const double Maths::Tau = 6.283185307;
const double Maths::RadToDeg = 180. / Maths::Pi;
const double Maths::DegToRad = Maths::Pi / 180.;

std::string GetFileExtension(const std::string loc);
std::string GetFileNoExtension(const std::string loc);

int Run(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	const int returnInt = Run(argc, argv);
	Log::EndLine();
	Log::WriteOneLine(std::to_string(returnInt));

	/*try {
		Run(argc, argv);
	}
	catch (int errorCode) {
		Log::WriteOneLine("Error Code: " + std::to_string(errorCode));
	}*/

	Log::Save();

	std::cout << '\a';

	std::cout << "\nPress enter to exit...\n";
	std::cin.ignore();
	//std::cout << '\a';
	//std::this_thread::sleep_for(std::chrono::seconds(1));

	return returnInt;
}

int Run(int argc, char* argv[]) {
#ifdef NDEBUG
	if (argc < 3) {
		Log::WriteOneLine("Drag and drop an image file, and a .json file");
		Log::WriteOneLine("Note: Only PNG, JPG, BMP or TGA image files are supported");

		return -1;
	}
#endif

	std::string imgLoc;
	std::string jsonLoc;

	// ----- GET FILES -----

#ifdef NDEBUG
	bool haveImg = false;
	bool haveJson = false;

	for (int i = 1; i < argc; i++) {
		std::string fileExtension = GetFileExtension(argv[i]);

		if (fileExtension == "json") {
			jsonLoc = argv[i];
			haveJson = true;
		} else if (fileExtension == "png" || fileExtension == "jpg" || fileExtension == "bmp" || fileExtension == "tga") {
			imgLoc = argv[i];
			haveImg = true;
		}
	}

	if (!(haveImg && haveJson)) {
		if (!haveImg) Log::WriteOneLine("Image file not found");
		if (!haveJson) Log::WriteOneLine("JSON file not found");

		return -1;
	}
#else
	jsonLoc = "data/dupes.json";
	imgLoc = "data/rubik.png";
#endif

	Log::WriteOneLine("Image: " + imgLoc);
	Log::WriteOneLine("JSON:  " + jsonLoc);
	Log::EndLine();

	// ----- READ FILES -----

	Image inputImg;
	if (!inputImg.Read(imgLoc.c_str(), 3)) return -1;

	Log::WriteOneLine("Width: " + std::to_string(inputImg.GetWidth()));
	Log::WriteOneLine("Height: " + std::to_string(inputImg.GetHeight()));
	Log::EndLine();

	std::ifstream f(jsonLoc);
	if (!(f)) {
		Log::WriteOneLine("Read failed: " + jsonLoc);
		return -1;
	}
	Log::WriteOneLine("Read success: " + jsonLoc);

	json settings = json::parse(f);

	const bool haveCount = settings.contains("count");
	const bool haveSeed = settings.contains("seed");
	const bool haveMaxIter = settings.contains("maxIter");
	const bool haveRedup = settings.contains("removeDuplicates");
	const bool haveTolerance = settings.contains("tolerance");
	const bool haveBandwidth = settings.contains("bandwidth");

	if (!(haveCount && haveSeed && haveMaxIter && haveRedup)) {
		if (!haveCount) Log::WriteOneLine("JSON setting not found: count");
		if (!haveSeed) Log::WriteOneLine("JSON setting not found: seed");
		if (!haveMaxIter) Log::WriteOneLine("JSON setting not found: maxIter");
		if (!haveRedup) Log::WriteOneLine("JSON setting not found: removeDuplicates");
		if (!haveTolerance) Log::WriteOneLine("JSON setting not found: tolerance");
		if (!haveBandwidth) Log::WriteOneLine("JSON setting not found: bandwidth");

		return -1;
	}

	const int count = settings["count"];
	Random::Seed = (unsigned int)settings["seed"];
	const unsigned int maxIter = (unsigned int)settings["maxIter"];
	const bool removeDuplicates = settings["removeDuplicates"];
	const double tolerance = (double)settings["tolerance"];
	const double bandwidth = (double)settings["bandwidth"];

	Log::WriteOneLine("Count: " + std::to_string(count));
	Log::WriteOneLine("Seed: " + std::to_string(Random::Seed));

	// ----- MAIN PROCESS -----

	// -- Get List of Colours --

	std::vector<Colour> colours;

	KMeans::GetColours(inputImg, colours, removeDuplicates);

	Log::WriteOneLine("Colour count in image: " + std::to_string(colours.size()));

	if (colours.size() <= count) {
		Log::WriteOneLine("Colour count is less than or equal to count setting");

		std::vector<OkLab> centers;
		centers.reserve(colours.size());
		for (size_t i = 0; i < colours.size(); i++) {
			centers.push_back(colours[i].GetOkLab());
		}

		KMeans::SavePalette(GetFileNoExtension(imgLoc), centers);
		return 0;
	}

	// -- Get Centers using Mean Shift Clustering --

	//const double bandwidth = MeanShift::EstimateBandwith(colours);

	{
		bool temp = true;
	}

	return 0;
}

std::string GetFileExtension(const std::string loc) {
	std::stringstream locStream(loc);
	std::string locSeg;
	std::vector<std::string> locSegList;

	while (std::getline(locStream, locSeg, '.')) locSegList.push_back(locSeg);

	std::string out = locSegList.back();

	std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) {
		return std::tolower(c);
	});

	return out;
}

std::string GetFileNoExtension(const std::string loc) {
	std::stringstream locStream(loc);
	std::string locSeg;
	std::vector<std::string> locSegList;

	while (std::getline(locStream, locSeg, '.')) locSegList.push_back(locSeg);

	std::string out = "";
	for (size_t i = 0; i < locSegList.size() - 1; i++) {
		if (i != 0) out += ".";
		out += locSegList[i];
	}

	return out;
}
