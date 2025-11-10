#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdint>

using std::vector;

namespace noise {
	double simplex(int x, int y);
	std::pair<double,double> skew(double x, double y);
	std::pair<int, int> tileIndex(double x, double y);
	inline double attenuate(double d) {return 1 - std::max(1/exp(d), 1.0);}
	vector<int> permutationTable(uint64_t seed);

	static const int8_t gradents[8][2] = {
		{  1,  0 }, { -1,  0 }, {  0,  1 }, {  0, -1 },
		{  1,  1 }, { -1,  1 }, {  1, -1 }, { -1, -1 }
	};

	inline int gradIndex(int hash) {
		return hash & 7; // 8 gradients
	}

	std::pair<int, int> tileIndex(double x, double y) {
		// Transform grid into lattice with 60deg angle between basis vectors
		//   Find correct tile (triangle shaped) by flooring coordinates
		//   which gives us the corner of the correct triangle that is
		//   closest to origo.

		constexpr double SKEW_CONSTANT = 0.366025403784; // (sqrt(3)-1)/2
		double s = (x + y) * SKEW_CONSTANT;
		int i = floor(x + s);
		int j = floor(y + s);
		
		return std::make_pair(i,j);
	}

	std::pair<double,double> skew(double x, double y) {
		constexpr double G2 = 0.211324865405; // (3-sqrt(3))/6

		auto [i,j] = tileIndex(x,y);

		double t = (i + j) * G2;
		double X0 = i - t;
		double Y0 = j - t;

		double x0 = x - X0;
		double y0 = y - Y0;
		return std::make_pair(x0,y0);
	}
}
	

