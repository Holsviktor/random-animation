#pragma once

#include <vector>
#include <cstdint>

using std::vector;

namespace noise {
	double simplex(int x, int y);
	std::pair<double,double> skew(double x, double y);
	std::pair<int, int> tileIndex(double x, double y);
	inline double attenuate(double d);
	vector<int> permutationTable(uint64_t seed);
}
