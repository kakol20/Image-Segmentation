#pragma once
#include <vector>

#include "../image/Image.h"
#include "Colour.h"

namespace KMeans
{
	void GetColours(const Image& image, std::vector<Colour>& colours);
}