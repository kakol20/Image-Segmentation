#pragma once
#include <vector>

#include "../image/Image.h"
#include "Colour.h"
#include "../colourSpace/OkLab.h"

namespace KMeans
{
	void GetColours(const Image& image, std::vector<Colour>& colours);

	void FirstCenter(const std::vector<Colour>& colours, std::vector<OkLab>& centers);

	extern bool TestDebug;

}