#pragma once

#include "../colourSpace/sRGB.hpp"
#include "../colourSpace/OkLab.h"

class Colour {
public:
	Colour();
	Colour(const uint8_t r, const uint8_t g, const uint8_t b);
	Colour(const sRGB& srgb);
	~Colour();

	// ----- ASSIGNMENT -----

	Colour& operator=(const Colour& other);

	// ----- COMPARISON -----

	bool operator==(const Colour& other) const {
		return m_r == other.m_r && m_g == other.m_g && m_b == other.m_b;
	}
	bool operator!=(const Colour& other) const {
		return !(this->operator==(other));
	}

	bool operator<(const Colour& other) const {
		if (Colour::Mode == Colour::SortMode::DISTANCE) return m_dist < other.m_dist;
		if (Colour::Mode == Colour::SortMode::FREQUENCY) return m_freq < other.m_freq;
		return m_dist < other.m_dist;
	};
	bool operator>(const Colour& other) const {
		if (Colour::Mode == Colour::SortMode::DISTANCE) return m_dist > other.m_dist;
		if (Colour::Mode == Colour::SortMode::FREQUENCY) return m_freq > other.m_freq;
		return m_dist > other.m_dist;
	};

	bool operator<=(const Colour& other) const {
		return this->operator<(other) || this->operator==(other);
	}
	bool operator>=(const Colour& other) const {
		return this->operator>(other) || this->operator==(other);
	}

	// ----- OTHER -----

	double GetDistance() const { return m_dist; };
	OkLab GetOkLab() const { return m_lab; };
	size_t GetCenterIndex() const { return m_centerIndex; };
	sRGB GetsRGB() const { return m_srgb; };
	std::string GetRGBUint() const;

	void StartCompare(const OkLab& lab, const size_t index);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="lab"></param>
	/// <param name="index"></param>
	/// <returns>Returns if changed</returns>
	bool Compare(const OkLab& lab, const size_t index);

	void AddFrequency();

	enum class SortMode {
		FREQUENCY, DISTANCE
	};
	static void SetSortMode(const Colour::SortMode mode);

private:
	sRGB m_srgb;
	OkLab m_lab;

	uint8_t m_r, m_g, m_b;

	size_t m_centerIndex;
	double m_dist;
	unsigned int m_freq;

	static Colour::SortMode Mode;
};