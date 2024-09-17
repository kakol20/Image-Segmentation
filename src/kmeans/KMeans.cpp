#include "KMeans.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

#include "../other/Log.h"

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

			const double r_double = (double)r_uint / 255.;
			const double g_double = (double)g_uint / 255.;
			const double b_double = (double)b_uint / 255.;

			const sRGB srgb(r_double, g_double, b_double);
			const Colour colour(srgb);

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
