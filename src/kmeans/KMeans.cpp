#include "KMeans.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

#include "../other/Log.h"
#include "../other/Random.h"

bool KMeans::TestDebug = true;

void KMeans::GetColours(const Image& image, std::vector<Colour>& colours) {
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

			if (std::find(colours.begin(), colours.end(), colour) == colours.end()) colours.push_back(colour);

			//colours.push_back(Colour(srgb));

			// -- Check Time --

			if (Log::CheckTimeSeconds(5.)) {
				double progress = double(image.GetIndex(x, y)) / double(image.GetSize());
				progress *= 100.;

				std::stringstream outStr;
				outStr << std::fixed << std::setprecision(6);

				if (progress < 10.) {
					outStr << " ";
				}
				outStr << progress;

				Log::EndLine();
				Log::StartLine();
				Log::Write("  ");
				Log::Write(outStr.str());
				Log::Write("%");

				Log::StartTime();
			}
		}
	}
	Log::EndLine();
}

void KMeans::FirstCenter(const std::vector<Colour>& colours, std::vector<OkLab>& centers) {
	const unsigned int randIndex = Random::RandUInt(0, colours.size() - 1);
	centers.push_back(colours[randIndex].GetOkLab());

	if (KMeans::TestDebug) {
		Log::WriteOneLine("Random First Point: rgb(" + colours[randIndex].GetsRGB().UintDebug() + ")");
	}
}