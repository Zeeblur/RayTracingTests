#include "utils.h"

namespace Utils
{
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq seed{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	extern std::mt19937_64 rng(seed);
	extern std::uniform_real_distribution<double> unif(0,1);

	//extern double getrand() { return unif(rng); }
}
