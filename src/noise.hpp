#pragma once

#include <vector>
#include <cstdint>
#include <functional>

using std::vector;

namespace noise {
	double simplex(double x, double y);
	double fbm(std::function<double(double,double)>f, double x, double y);
	std::pair<int,int> gridSkew(double x, double y);
	void initPermutationTable(uint64_t seed);
	int cornerGradient(int i, int j);
	double dot(double* a, double* b);
	inline double attenuate(double d);
}
