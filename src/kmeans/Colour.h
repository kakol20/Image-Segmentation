#pragma once

#include "../colourSpace/sRGB.hpp"
#include "../colourSpace/OkLab.h"

class Colour {
public:
	Colour();
	Colour(const sRGB& srgb);
	~Colour();

	// ----- ASSIGNMENT -----

	Colour& operator=(const Colour& other);

	// ----- COMPARISON -----

	inline bool operator==(const Colour& other) const {
		return m_srgb == other.m_srgb;
	}
	inline bool operator!=(const Colour& other) const {
		return !(this->operator==(other));
	}

	inline bool operator<(const Colour& other) const {
		return m_dist < other.m_dist;
	}
	inline bool operator>(const Colour& other) const {
		return m_dist > other.m_dist;
	}

	inline bool operator<=(const Colour& other) const {
		return this->operator<(other) || this->operator==(other);
	}
	inline bool operator>=(const Colour& other) const {
		return this->operator>(other) || this->operator==(other);
	}

private:
	sRGB m_srgb;
	OkLab m_lab;

	size_t m_centerIndex;
	double m_dist;
};