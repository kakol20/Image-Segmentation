#pragma once
#include <vector>

#include "../image/Image.h"
#include "Colour.h"
#include "../colourSpace/OkLab.h"

namespace KMeans
{
	void GetColours(const Image& image, std::vector<Colour>& colours, const bool removeDupes = true);

	void FirstCenter(std::vector<Colour>& colours, std::vector<OkLab>& centers, const bool removeDupes);
	void NewCenter(const std::vector<Colour>& colours, std::vector<OkLab>& centers);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="colours"></param>
	/// <param name="centers"></param>
	/// <param name="debug"></param>
	/// <returns>Returns if any point has been changed</returns>
	bool SortColours(std::vector<Colour>& colours, const std::vector<OkLab>& centers, const bool debug = false);

	void MoveCenters(const std::vector<Colour>& colours, std::vector<OkLab>& centers);

	void SavePalette(const std::string& loc, const std::vector<OkLab>& centers);

	extern bool TestDebug;

}