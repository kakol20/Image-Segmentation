#include "MeanShift.h"

#include <algorithm>

#include "../other/Log.h"


Colour MeanShift::ComputeMean(const std::vector<Colour>& colours, const Colour& center, const double bandwidth) {
	std::vector<Colour> inBand;
	for (const auto& col : colours) {
		if (Colour::LabDistance(center, col) <= bandwidth) inBand.push_back(col);
	}

	if (inBand.empty()) return center;

	Colour sum = Colour(0, 0, 0);
	for (const auto& col : inBand) sum = sum + col;

	sum /= double(inBand.size());
	sum.UpdateRGBFromLab();
	return sum;
}

std::vector<Colour> MeanShift::MergeCenters(const std::vector<Colour>& centers, const double tolerance) {
	std::vector<Colour> merged;

	for (const auto& c : centers) {
		bool found = false;
		for (const auto& m : merged) {
			if (Colour::LabDistance(c, m) < tolerance) {
				found = true;
				break;
			}
		}
		if (!found) merged.push_back(c);
	}
	return merged;
}

void MeanShift::Run(const std::vector<Colour>& colours, std::vector<Colour>& centers, const double bandwidth, const double tolerance, const unsigned int maxIter) {
	std::vector<Colour> shifted = colours;

	// Shift each point

	Log::StartTime();

	Log::EndLine();
	Log::StartLine();
	Log::Write("Getting centers...");

	for (size_t i = 0; i < shifted.size(); i++) {
		Colour prev = shifted[i];
		unsigned int iter = 0;

		while (iter++ < 32) {
			Colour next = MeanShift::ComputeMean(colours, prev, bandwidth);
			if (Colour::LabDistance(next, prev) < tolerance) break;
			prev = next;
		}
		shifted[i] = prev;

		// -- Check Time --

		if (Log::CheckTimeSeconds(5.)) {
			double progress = double(i) / double(shifted.size());
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
	Log::EndLine();

	// Merge final positions into cluster centers
	centers = MeanShift::MergeCenters(shifted, tolerance);
}
