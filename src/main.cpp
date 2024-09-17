#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../ext/json/json.hpp"
#include "colourSpace/ColorSpace.hpp"
#include "colourSpace/OkLab.h"
#include "colourSpace/OkLCh.h"
#include "colourSpace/sRGB.hpp"
#include "image/Image.h"
#include "kmeans/Colour.h"
#include "kmeans/KMeans.h"
#include "maths/Maths.hpp"
#include "other/Log.h"
#include "other/Random.h"

using json = nlohmann::json;

const double Maths::Pi = 3.1415926535;
const double Maths::Tau = 6.283185307;
const double Maths::RadToDeg = 180. / Maths::Pi;
const double Maths::DegToRad = Maths::Pi / 180.;

std::string GetFileExtension(const std::string loc);
std::string GetFileNoExtension(const std::string loc);

int main(int argc, char* argv[]) {
	if (argc < 3) {
		Log::WriteOneLine("Drag and drop an image file, and a .json file");
		Log::WriteOneLine("Note: Only PNG, JPG, BMP or TGA image files are supported");

		Log::Save();
		std::cout << "\nPress enter to exit...\n";
		std::cin.ignore();
		return 0;
	}

	std::string imgLoc;
	bool haveImg = false;

	std::string jsonLoc;
	bool haveJson = false;

	// ----- GET FILES -----

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

		Log::Save();
		std::cout << "\nPress enter to exit...\n";
		std::cin.ignore();
		return 0;
	}

	Log::WriteOneLine("Image: " + imgLoc);
	Log::WriteOneLine("JSON:  " + jsonLoc);
	Log::EndLine();

	// ----- READ FILES -----

	Image inputImg;
	if (!inputImg.Read(imgLoc.c_str(), 3)) {
		Log::Save();
		std::cout << "\nPress enter to exit...\n";
		std::cin.ignore();
		return 0;
	}
	Log::WriteOneLine("Width: " + std::to_string(inputImg.GetWidth()));
	Log::WriteOneLine("Height: " + std::to_string(inputImg.GetHeight()));
	Log::EndLine();

	std::ifstream f(jsonLoc);
	if (!(f)) {
		Log::WriteOneLine("Read failed: " + jsonLoc);
		Log::Save();
		std::cout << "\nPress enter to exit...\n";
		std::cin.ignore();
		return 0;
	}
	Log::WriteOneLine("Read success: " + jsonLoc);

	json settings = json::parse(f);

	const bool haveCount = settings.contains("count");
	const bool haveSeed = settings.contains("seed");

	if (!(haveCount && haveSeed)) {
		if (!haveCount) Log::WriteOneLine("JSON setting not found: count");
		if (!haveSeed) Log::WriteOneLine("JSON setting not found: seed");

		Log::Save();
		std::cout << "\nPress enter to exit...\n";
		std::cin.ignore();
		return 0;
	}

	const int count = settings["count"];
	Random::Seed = (unsigned int)settings["seed"];

	Log::WriteOneLine("Count: " + std::to_string(count));
	Log::WriteOneLine("Seed: " + std::to_string(Random::Seed));

	// ----- MAIN PROCESS -----

	//KMeans::TempDebug = true;

	// -- Get List of Colours

	std::vector<Colour> colours;

	KMeans::GetColours(inputImg, colours);

	Log::WriteOneLine("Colour count in image: " + std::to_string(colours.size()));

	if (colours.size() <= count) {
		Log::WriteOneLine("Colour count is less than or equal to count setting");
		
		/*
		TODO: 
			Save palette file
		*/
	}

	// -- Get Centers --

	std::vector<OkLab> centers;
	KMeans::FirstCenter(colours, centers);

	KMeans::SortColours(colours, centers, true);

	Log::Save();

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
