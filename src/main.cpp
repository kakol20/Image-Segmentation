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
	const bool haveMaxIter = settings.contains("maxIter");

	if (!(haveCount && haveSeed && haveMaxIter)) {
		if (!haveCount) Log::WriteOneLine("JSON setting not found: count");
		if (!haveSeed) Log::WriteOneLine("JSON setting not found: seed");
		if (!haveMaxIter) Log::WriteOneLine("JSON setting not found: maxIter");

		Log::Save();
		std::cout << "\nPress enter to exit...\n";
		std::cin.ignore();
		return 0;
	}

	const int count = settings["count"];
	Random::Seed = (unsigned int)settings["seed"];
	const unsigned int maxIter = (unsigned int)settings["maxIter"];

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

		std::vector<OkLab> centers;
		centers.reserve(colours.size());
		for (size_t i = 0; i < colours.size(); i++) {
			centers.push_back(colours[i].GetOkLab());
		}
		
		KMeans::SavePalette(GetFileNoExtension(imgLoc), centers);

		Log::Save();
		std::cout << "\nPress enter to exit...\n";
		std::cin.ignore();
		return 0;
	}

	// -- Get Centers --

	std::vector<OkLab> centers;
	centers.reserve((size_t)count);

	KMeans::FirstCenter(colours, centers);
	KMeans::SortColours(colours, centers, true);

	Log::EndLine();
	Log::WriteOneLine("Adding centers...");
	Log::StartTime();
	for (int i = 1; i < count; i++) {
		KMeans::NewCenter(colours, centers);
		KMeans::SortColours(colours, centers, false);

		if (Log::CheckTimeSeconds(5.0)) {
			const double process = ((double)i / (double)count) * 100;
			std::string processStr = Log::ToString(process, 6);
			processStr = Log::LeadingCharacter(processStr, 9);

			Log::WriteOneLine("  " + processStr + "%");

			Log::StartTime();
		}
	}

	Log::EndLine();
	Log::WriteOneLine("Before Movement");
	Log::WriteOneLine("Centers Count: " + Log::ToString(centers.size()));
	KMeans::SortColours(colours, centers, true);

	Log::EndLine();
	Log::WriteOneLine("Unmoved Centers");
	for (size_t i = 0; i < centers.size(); i++) {
		const sRGB srgb = OkLab::OkLabtosRGB(centers[i]);
		const std::string hex = srgb.sRGBtoHex();
		const std::string rgb = srgb.UintDebug();

		Log::WriteOneLine("#" + hex + " - rgb(" + rgb + ")");
	}

	// -- Move Centers --
	
	Log::EndLine();
	Log::WriteOneLine("Moving Centers...");

	unsigned int iterationCount = 0;
	const std::string maxIterStr = Log::ToString(maxIter);

	Log::StartTime();
	while (true) {
		iterationCount++;

		KMeans::MoveCenters(colours, centers);

		if (!KMeans::SortColours(colours, centers)) break;
		if (iterationCount >= maxIter) break;

		if (Log::CheckTimeSeconds(5.)) {
			std::string progress = Log::ToString(iterationCount, (unsigned int)maxIterStr.size(), ' ');

			Log::WriteOneLine("  Iteration Count: " + progress);
			Log::StartTime();
		}
	}
	Log::WriteOneLine("Took " + Log::ToString(iterationCount) + " iterations");
	
	// ----- SAVE PALETTE FILE -----

	Log::EndLine();
	Log::WriteOneLine("After Movement");
	std::string extraChange = KMeans::SortColours(colours, centers, true) ? "TRUE" : "FALSE";
	if (KMeans::TestDebug) Log::WriteOneLine("Extra Change: " + extraChange);
	Log::EndLine();

	Log::WriteOneLine("Moved Centers");
	for (size_t i = 0; i < centers.size(); i++) {
		const sRGB srgb = OkLab::OkLabtosRGB(centers[i]);
		const std::string hex = srgb.sRGBtoHex();
		const std::string rgb = srgb.UintDebug();

		Log::WriteOneLine("#" + hex + " - rgb(" + rgb + ")");
	}
	KMeans::SavePalette(GetFileNoExtension(imgLoc), centers);

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
