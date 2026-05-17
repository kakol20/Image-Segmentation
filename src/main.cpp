#include <iostream>

#ifdef _DEBUG
const bool debug = true;
#else
const bool debug = false;
#endif // _DEBUG

#ifdef WIN32
const bool win32 = true;
#else
const bool win32 = false;
#endif // WIN32

#include "other/Log.h"
#include "other/Random.h"

int main() {
	Random::EpochSeed();
	Log::WriteOneLine("Random Seed: " + Log::ToString(Random::Seed));
	Log::WriteOneLine("Random Float: " + Log::ToString(Random::RandDouble()));
	Log::WriteOneLine("Random UInt: " + Log::ToString(Random::Rand()));
	Log::WriteOneLine("Roll 1d10+2 & 1d4: " + Log::ToString(Random::RandUInt(1, 10) + 2 + Random::RandUInt(1, 4)));
	Log::Sound(1);
	Log::HoldConsole();

	return 0;
}