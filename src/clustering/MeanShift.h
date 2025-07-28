#pragma once

#include <vector>

#include "../image/Image.h"
#include "Colour.h"
#include "../colourSpace/OkLab.h"

namespace MeanShift {
	Colour ComputeMean(const std::vector<Colour>& colours, const Colour& center, const double bandwidth);

	std::vector<Colour> MergeCenters(const std::vector<Colour>& centers, const double tolerance);

	void Run(const std::vector<Colour>& colours, std::vector<Colour>& centers, const double bandwidth, const double tolerance, const unsigned int maxIter);
}