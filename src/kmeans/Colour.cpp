#include "Colour.h"

#include "../other/Log.h"

Colour::SortMode Colour::Mode = Colour::SortMode::DISTANCE;

Colour::Colour() {
	m_centerIndex = 0;
	m_dist = 100.;
	m_oldCenterIndex = 0;

	m_r = 0;
	m_g = 0;
	m_b = 0;
	m_freq = 0;
}

Colour::Colour(const sRGB& srgb) {
	m_centerIndex = 0;
	m_dist = 100.;
	m_oldCenterIndex = 0;

	m_srgb = srgb;
	m_lab = OkLab::sRGBtoOkLab(srgb);

	m_r = m_srgb.GetRUInt();
	m_g = m_srgb.GetGUInt();
	m_b = m_srgb.GetBUInt();
	m_freq = 0;
}

Colour::~Colour() {
}

Colour& Colour::operator=(const Colour& other) {
	if (this == &other) return *this;

	m_srgb = other.m_srgb;
	m_lab = other.m_lab;

	m_centerIndex = other.m_centerIndex;
	m_dist = other.m_dist;
	m_oldCenterIndex = other.m_centerIndex;

	m_r = m_srgb.GetRUInt();
	m_g = m_srgb.GetGUInt();
	m_b = m_srgb.GetBUInt();

	m_freq = other.m_freq;

	return *this;
}

Colour::Colour(const uint8_t r, const uint8_t g, const uint8_t b) {
	m_r = r;
	m_g = g;
	m_b = b;

	m_centerIndex = 0;
	m_dist = 100.;
	m_oldCenterIndex = 0;

	m_srgb = sRGB((double)r / 255., (double)g / 255., (double)b / 255.);
	m_lab = OkLab::sRGBtoOkLab(m_srgb);
	m_freq = 0;
}

std::string Colour::GetRGBUint() const {
	return Log::ToString((size_t)m_r, 3, ' ') + " " + Log::ToString((size_t)m_g, 3, ' ') + " " + Log::ToString((size_t)m_b, 3, ' ');
}

void Colour::StartCompare(const OkLab& lab, const size_t index) {
	m_centerIndex = index;
	m_dist = OkLab::Distance(m_lab, lab);
}

void Colour::Compare(const OkLab& lab, const size_t index) {
	const double dist = OkLab::Distance(m_lab, lab);

	if (dist < m_dist) {
		m_centerIndex = index;
		m_dist = dist;
	}

	//return changed;
}

void Colour::AddFrequency() {
	m_freq += 1;
}

void Colour::SetSortMode(const Colour::SortMode mode) {
	Colour::Mode = mode;
}
