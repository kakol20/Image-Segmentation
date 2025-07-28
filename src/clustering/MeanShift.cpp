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

std::vector<Colour> MeanShift::MergeCenters(const std::vector<Colour>& centers, double tolerance) {
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
