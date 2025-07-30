#include "MeanShift.h"

#include <algorithm>

#include "../other/Log.h"
//
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
//
//void MeanShift::Run(const std::vector<Colour>& colours, std::vector<Colour>& centers, const double bandwidth, const double tolerance, const unsigned int maxIter) {
//	std::vector<Colour> shifted = colours;
//
//	// Shift each point
//
//	Log::StartTime();
//
//	Log::EndLine();
//	Log::StartLine();
//	Log::Write("Getting centers...");
//
//	auto n = std::thread::hardware_concurrency();
//
//	{
//		bool temp = true;
//	}
//
//	//for (size_t i = 0; i < shifted.size(); i++) {
//	//	Colour prev = shifted[i];
//	//	unsigned int iter = 0;
//
//	//	while (iter++ < 32) {
//	//		Colour next = MeanShift::ComputeMean(colours, prev, bandwidth);
//	//		if (Colour::LabDistance(next, prev) < tolerance) break;
//	//		prev = next;
//	//	}
//	//	shifted[i] = prev;
//
//	//	// -- Check Time --
//
//	//	if (Log::CheckTimeSeconds(5.)) {
//	//		double progress = double(i) / double(shifted.size());
//	//		progress *= 100.;
//
//	//		std::string outStr = Log::ToString(progress, 6);
//	//		outStr = Log::LeadingCharacter(outStr, 9);
//
//	//		Log::EndLine();
//	//		Log::StartLine();
//	//		Log::Write("  ");
//	//		Log::Write(outStr);
//	//		Log::Write("%");
//
//	//		Log::StartTime();
//	//	}
//	//}
//	//Log::EndLine();
//
//	// Merge final positions into cluster centers
//	centers = MeanShift::MergeCenters(shifted, tolerance);
//}
//

MeanShift::MeanShift() {
	m_progress = 0;
}

void MeanShift::Run(const std::vector<Colour>& colours, std::vector<Colour>& centers, const double bandwidth, const double tolerance, const unsigned int maxIter) {
	std::vector<Colour> shifted = colours;

	// Shift each point

	Log::StartTime();

	Log::EndLine();
	Log::StartLine();
	Log::Write("Getting centers...");

	const unsigned int hc = std::thread::hardware_concurrency() == 0 ? 1 : std::thread::hardware_concurrency();
	const unsigned int n = hc > 2 ? hc - 2 : hc;

	// -- Run Threads --

	if ((unsigned int)shifted.size() <= n) {
		MeanShift::Loop(shifted, colours, bandwidth, tolerance, maxIter, 0, shifted.size());
	} else {
		Log::EndLine();
		Log::StartLine();
		Log::Write("Threads used: " + Log::ToString(n));

		std::vector<std::thread> threads;

		bool allFinished = false;

		const size_t length = shifted.size();
		const size_t chunkSize = (length + n - 1) / n;

		for (int i = 0; i < (int)n; i++) {
			size_t start = i * chunkSize;
			size_t end = std::min(start + chunkSize, length);

			if (start >= end) break;

			// Loop(std::vector<Colour>& shifted, const std::vector<Colour>& colours, const double bandwidth, const double tolerance, const unsigned int maxIter, const size_t start, const size_t end);
			threads.emplace_back(MeanShift::Loop, std::ref(shifted), colours, bandwidth, tolerance, maxIter, start, end);

			threads.back().detach();
			MeanShift::m_threadStatus[threads.back().get_id()] = false;
		}

		while (true) {
			allFinished = true;
			for (auto it = MeanShift::m_threadStatus.begin(); it != MeanShift::m_threadStatus.end(); it++) {
				if (!it->second) allFinished = false;
			}

			if (allFinished) {
				for (auto it = threads.begin(); it != threads.end(); it++) it->join();
				break;
			}

			// -- Check Time --
			if (Log::CheckTimeSeconds(5.)) {
				MeanShift::m_mutex.lock();

				double percent = double(MeanShift::m_progress) / double(length);
				percent *= 100.;

				MeanShift::m_mutex.unlock();

				std::string outStr = Log::ToString(percent, 6);
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
	}

	// Merge final positions into cluster centers
	centers = MeanShift::MergeCenters(shifted, tolerance);
}

void MeanShift::Loop(std::vector<Colour>& shifted, const std::vector<Colour>& colours, const double bandwidth, const double tolerance, const unsigned int maxIter, const size_t start, const size_t end) {
	for (size_t i = start; i < end; i++) {
		Colour prev = shifted[i];

		unsigned int iter = 0;
		while (iter++ < maxIter) {
			Colour next = MeanShift::ComputeMean(colours, prev, bandwidth);
			if (Colour::LabDistance(next, prev) < tolerance) break;
			prev = next;
		}

		MeanShift::m_mutex.lock();
		shifted[i] = prev;
		MeanShift::m_progress++;
		MeanShift::m_mutex.unlock();
	}

	MeanShift::m_mutex.lock();
	MeanShift::m_threadStatus[std::this_thread::get_id()] = true;
	MeanShift::m_mutex.unlock();
}