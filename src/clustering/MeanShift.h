#pragma once

#include <vector>

#include "../image/Image.h"
#include "Colour.h"
#include "../colourSpace/OkLab.h"

namespace MeanShift {
	/// <summary>
	/// Median of Pairwise Distances
	/// </summary>
	/// <param name="colours"></param>
	/// <param name="lightMode"></param>
	/// <returns></returns>
	//double EstimateBandwith(const std::vector<Colour>& colours, const bool lightMode = false);

	Colour ComputeMean(const std::vector<Colour>& colours, const Colour& center, const double bandwidth);
}