#include "Colour.h"

Colour::Colour() {
	m_centerIndex = 0;
	m_dist = 0.;
}

Colour::Colour(const sRGB& srgb) {
	m_centerIndex = 0;
	m_dist = 0.;

	m_srgb = srgb;
	m_lab = OkLab::sRGBtoOkLab(srgb);
}

Colour::~Colour() {
}

Colour& Colour::operator=(const Colour& other) {
	if (this == &other) return *this;

	m_srgb = other.m_srgb;
	m_lab = other.m_lab;

	m_centerIndex = other.m_centerIndex;
	m_dist = other.m_dist;

	return *this;
}
