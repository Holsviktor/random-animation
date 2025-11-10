#pragma once

#include <vector>
#include <cstdint>

using std::vector;

namespace noise {
	double simplex(int x, int y);
	std::pair<int,int> gridSkew(double x, double y);
	vector<int> permutationTable(uint64_t seed);
	vector<vector<double>> cornerDistances(double x, double y);
	vector<int> permutationTable(uint64_t seed);
}
