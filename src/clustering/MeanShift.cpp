#include "MeanShift.h"

#include <algorithm>

#include "../other/Log.h"

//double MeanShift::EstimateBandwith(const std::vector<Colour>& colours, const bool lightMode) {
//	std::vector<double> distances;
//
//	const size_t coloursSize = colours.size();
//	const size_t reserveSize = coloursSize * (coloursSize - 1) / 2;
//	const size_t reserveL = Log::ToString(reserveSize).size();
//	distances.reserve(reserveSize);
//
//	Log::StartTime();
//
//	Log::EndLine();
//	/*Log::StartLine();
//	Log::Write("Estimating Bandwith...");
//	Log::EndLine();*/
//	Log::WriteOneLine("Estimating Bandwith...");
//
//
//	Log::StartLine();
//	Log::Write("Calculating Distances");
//
//	size_t progress = 0;
//
//
//
//	for (size_t i = 0; i < coloursSize; i++) {
//		for (size_t j = i + 1; j < coloursSize; j++) {
//			//distances.push_back(Colour::LabDistance(colours[i], colours[j]));
//
//			const double distance = Colour::LabDistance(colours[i], colours[j]);
//
//			auto pos = std::lower_bound(distances.begin(), distances.end(), distance);
//			distances.insert(pos, distance);
//
//			progress++;
//
//			if (Log::CheckTimeSeconds(5.)) {
//				double prog = double(progress) / double(reserveSize);
//				prog *= 100.;
//
//				//std::string outStr = Log::ToString(prog, 6);
//				//outStr = Log::LeadingCharacter(outStr, 9);
//
//				std::string outStr = Log::ToString(progress);
//				outStr = Log::LeadingCharacter(outStr, (unsigned int)(reserveL), ' ');
//
//				Log::EndLine();
//				Log::StartLine();
//				Log::Write("  ");
//				Log::Write(outStr);
//				Log::Write(" / ");
//				Log::Write(Log::ToString(reserveSize));
//				Log::Write(" = ");
//				Log::Write(Log::LeadingCharacter(Log::ToString(prog, 6), 9));
//				Log::Write("%");
//
//				Log::StartTime();
//			}
//		}
//	}
//	Log::EndLine();
//
//	if (distances.empty()) return 1.0;
//
//	Log::WriteOneLine("Amount: " + Log::ToString(distances.size()));
//
//
//	// ----- CUSTOM BUBBLE SORT -----
//	//Log::WriteOneLine("Sorting distances using std::sort - please wait");
//	//std::sort(distances.begin(), distances.end());
//
//	double out = distances[distances.size() / 2];
//	Log::WriteOneLine("Bandwidth: " + Log::ToString(out, 6));
//
//	return out;
//}

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

	bool temp = 0;

	return Colour();
}
