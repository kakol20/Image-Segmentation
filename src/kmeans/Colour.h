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
		return m_dist < other.m_dist;
	}
	bool operator>(const Colour& other) const {
		return m_dist > other.m_dist;
	}

	bool operator<=(const Colour& other) const {
		return this->operator<(other) || this->operator==(other);
	}
	bool operator>=(const Colour& other) const {
		return this->operator>(other) || this->operator==(other);
	}

	// ----- OTHER -----

	OkLab GetOkLab() const { return m_lab; };
	sRGB GetsRGB() const { return m_srgb; };

private:
	sRGB m_srgb;
	OkLab m_lab;

	uint8_t m_r, m_g, m_b;

	size_t m_centerIndex;
	double m_dist;
};