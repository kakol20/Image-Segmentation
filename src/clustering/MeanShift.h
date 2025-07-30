#pragma once

#include <mutex>
#include <vector>
#include <thread>
#include <map>

#include "../colourSpace/OkLab.h"
#include "../image/Image.h"
#include "Colour.h"


class MeanShift {
public:
	MeanShift();
	~MeanShift() {};

	static void Run(const std::vector<Colour>& colours, std::vector<Colour>& centers, const double bandwidth, const double tolerance, const unsigned int maxIter);

private:
	static std::mutex m_mutex;
	static int m_progress;
	static std::map<std::thread::id, bool> m_threadStatus;

	static Colour ComputeMean(const std::vector<Colour>& colours, const Colour& center, const double bandwidth);
	static std::vector<Colour> MergeCenters(const std::vector<Colour>& centers, const double tolerance);
	static void Loop(std::vector<Colour>& shifted, const std::vector<Colour>& colours, const double bandwidth, const double tolerance, const unsigned int maxIter, const size_t start, const size_t end);
};