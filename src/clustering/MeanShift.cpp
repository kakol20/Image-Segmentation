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
