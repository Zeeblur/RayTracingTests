#pragma once
#include <random>
#include <chrono>

namespace Utils
{
	extern std::mt19937_64 rng;
	extern std::uniform_real_distribution<double> unif;

	//extern double getrand();
}