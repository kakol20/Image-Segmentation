#pragma once

class Random {
public:
	Random() {};
	~Random() {};

	static unsigned int Rand();
	static unsigned int RandUInt(unsigned int min, unsigned int max);

	static unsigned int Seed;

private:
	static unsigned int MaxRand;
};