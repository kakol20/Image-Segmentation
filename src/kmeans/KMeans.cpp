#include "KMeans.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>

#include "../other/Log.h"
#include "../other/Random.h"

bool KMeans::TestDebug = true;

void KMeans::GetColours(const Image& image, std::vector<Colour>& colours, const bool removeDupes) {
	Log::StartTime();

	Log::EndLine();
	Log::StartLine();
	Log::Write("Getting pixel colours...");
	for (int y = 0; y < image.GetHeight(); y++) {
		for (int x = 0; x < image.GetWidth(); x++) {
			const size_t index = image.GetIndex(x, y);

			const uint8_t r_uint = image.GetData(index + 0);
			const uint8_t g_uint = image.GetData(index + 1);
			const uint8_t b_uint = image.GetData(index + 2);

			//const sRGB srgb(r_double, g_double, b_double);
			const Colour colour(r_uint, g_uint, b_uint);

			if (removeDupes) {
				auto found = std::find(colours.begin(), colours.end(), colour);
				if (found == colours.end()) {
					colours.push_back(colour);
				} else {
					found->AddFrequency();
				}
			} else {
				colours.push_back(colour);
			}

			// -- Check Time --

			if (Log::CheckTimeSeconds(5.)) {
				double progress = double(image.GetIndex(x, y)) / double(image.GetSize());
				progress *= 100.;

				std::string outStr = Log::ToString(progress, 6);
				outStr = Log::LeadingCharacter(outStr, 9);

				Log::EndLine();
				Log::StartLine();
				Log::Write("  ");
				Log::Write(outStr);
				Log::Write("%");

				Log::StartTime();
			}
		}
	}
	Log::EndLine();
}

void KMeans::FirstCenter(std::vector<Colour>& colours, std::vector<OkLab>& centers, const bool removeDupes = false) {
	if (!removeDupes) {
		const size_t randomIndex = (size_t)Random::RandUInt(0, (unsigned int)colours.size() - 1);
		centers.push_back(colours[randomIndex].GetOkLab());

		if (KMeans::TestDebug) {
			Log::EndLine();
			Log::WriteOneLine("First Point: rgb(" + colours[randomIndex].GetsRGB().UintDebug() + ")");
		}

		Colour::SetSortMode(Colour::SortMode::DISTANCE);
	} else {
		Colour::SetSortMode(Colour::SortMode::FREQUENCY);

		std::sort(colours.begin(), colours.end(), std::greater<Colour>());
		centers.push_back(colours.front().GetOkLab());

		if (KMeans::TestDebug) {
			Log::EndLine();
			Log::WriteOneLine("First Point: rgb(" + colours.front().GetsRGB().UintDebug() + ")");
		}

		Colour::SetSortMode(Colour::SortMode::DISTANCE);
	}
}

void KMeans::NewCenter(const std::vector<Colour>& colours, std::vector<OkLab>& centers) {
	centers.push_back(colours.front().GetOkLab());
}

bool KMeans::SortColours(std::vector<Colour>& colours, const std::vector<OkLab>& centers, const bool debug) {
	bool changed = false;
	for (size_t i = 0; i < colours.size(); i++) {
		for (size_t j = 0; j < centers.size(); j++) {
			const bool compare = colours[i].Compare(centers[j], j);
			changed = compare || changed;
		}
	}

	std::sort(colours.begin(), colours.end(), std::greater<Colour>());

	if (debug) {
		const unsigned int intPrecision = (unsigned int)(std::to_string(colours.size())).size();
		//Log::EndLine();

		Log::StartLine();
		Log::Write("  Front Colour: rgb(" + colours.front().GetRGBUint() + "), ");
		Log::Write("Distance: " + Log::ToString(colours.front().GetDistance()) + ", ");
		Log::Write("Center Index: " + Log::ToString(colours.front().GetCenterIndex(), intPrecision));
		Log::EndLine();

		Log::StartLine();
		Log::Write("  Back Colour : rgb(" + colours.back().GetRGBUint() + "), ");
		Log::Write("Distance: " + Log::ToString(colours.back().GetDistance()) + ", ");
		Log::Write("Center Index: " + Log::ToString(colours.back().GetCenterIndex(), intPrecision));
		Log::EndLine();
	}

	return changed;
}

void KMeans::MoveCenters(const std::vector<Colour>& colours, std::vector<OkLab>& centers) {
	std::vector<OkLab> average(centers.size());
	std::vector<unsigned int> centerCount(centers.size());

	for (size_t i = 0; i < colours.size(); i++) {
		const size_t centerIndex = colours[i].GetCenterIndex();

		average[centerIndex] += colours[i].GetOkLab();
		centerCount[centerIndex] += 1;
	}

	for (size_t i = 0; i < centers.size(); i++) {
		average[i] /= (double)centerCount[i];
		centers[i] = average[i];
	}
}

void KMeans::SavePalette(const std::string& loc, const std::vector<OkLab>& centers) {
	const std::string paletteLoc = loc + ".palette";

	std::fstream paletteFile;
	paletteFile.open(paletteLoc, std::ios_base::out);

	for (size_t i = 0; i < centers.size(); i++) {
		if (i > 0) paletteFile << '\n';
		const sRGB srgb = OkLab::OkLabtosRGB(centers[i]);

		paletteFile << srgb.sRGBtoHex();
	}
	paletteFile.close();
}