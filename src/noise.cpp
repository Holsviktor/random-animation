#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <random>
#include <functional>

using std::vector;

#define SKEW_CONSTANT  0.366025403784 // (sqrt(3)-1)/2
#define INV_SKW_CONST  0.211324865405 // (3-sqrt(3))/6
#define SQRT_2 1.4142135623730951
		 
inline double vectorLength(double x, double y) {
	return std::pow(x,2) + std::pow(y,2);
}
namespace noise {
	double simplex(double x, double y);
	double fbm(std::function<double(double,double)>f, double x, double y);

	std::pair<int,int> gridSkew(double x, double y);
	void initPermutationTable(uint64_t seed);
	int cornerGradient(int i, int j);
	double dot(double* a, double* b);
	inline double attenuate(double d) {return 1 - std::max(1/exp(d), 1.0);}

	static const double gradients[8][2] = {
		{  1,  0 }, { -1,  0 }, {  0,  1 }, {  0, -1 },
		{  1/SQRT_2,  1/SQRT_2 }, { -1/SQRT_2,  1/SQRT_2 }, {  1/SQRT_2, -1/SQRT_2 }, { -1/SQRT_2, -1/SQRT_2 }
	};

	vector<int> permutation(512);

	int cornerGradient(int i, int j) {
		return permutation[ permutation[i & 255]+ (j & 255) ] & 7;
	}

	void initPermutationTable(uint64_t seed) {
		std::mt19937 prng(seed);
		std::iota(permutation.begin(), permutation.end(),0);
		std::shuffle(permutation.begin(), permutation.end(), prng);
		for (int i = 256; i < 512; i++) {
			permutation[i] = permutation[i&255]&255;
		}
	}
	
	inline double dot(double* a, double* b) {
		return ((double) a[0]) * b[0] + ((double) a[1]) * b[1];
	}

	std::pair<int, int> gridSkew(double x, double y) {
		// Transform grid into lattice with 60deg angle between basis vectors
		//   Find correct tile (triangle shaped) by flooring coordinates
		//   which gives us the corner of the correct triangle that is
		//   closest to origo.

		
		double s = (x + y) * SKEW_CONSTANT;
		int i = floor(x + s);
		int j = floor(y + s);
		
		return std::make_pair(i,j);
	}

	std::pair<double,double> unskew(double x, double y) {
		double t = (x + y) * INV_SKW_CONST;
		double X = x - t;
		double Y = y - t;
		return std::make_pair(X, Y);
		
	}

	double simplex(double x, double y) {
		double value = 0;
		// Find coordinates of upper left corner
		auto [i,j] = gridSkew(x,y);
		auto [corner_x, corner_y] = unskew(i,j);

		// Use that to find all corners
		double corner_x_offset = x - corner_x;
		double corner_y_offset = y - corner_y;
		int in_lower = (corner_x_offset > corner_y_offset);

		int corner_indexes[3][2] = {{i         ,j          },
							 		{i+in_lower,j+!in_lower},
							 		{i+1       ,j+1        }};
		// Find the gradient at each corner
		double corner_gradient[3][2];
		for (int i = 0; i < 3; i++) {
			int gradient_index = cornerGradient(corner_indexes[i][0], corner_indexes[i][1]);
			corner_gradient[i][0] = gradients[gradient_index][0];
			corner_gradient[i][1] = gradients[gradient_index][1];
		}

		// Calculate distance to all corners
		double corner_distances2[3][2];
		vector<double> weight(3,0.0);

		for (int i = 0; i < 3; i++) {
			auto [skewed_corner_x, skewed_corner_y] = corner_indexes[i];	
			auto [corner_x, corner_y] = unskew(skewed_corner_x, skewed_corner_y);
			corner_distances2[i][0] = (x - corner_x);
			corner_distances2[i][1] = (y - corner_y);
			weight[i] = 0.5 - (vectorLength((x-corner_x), y-corner_y));

			value += (weight[i] > 0) 
				   * std::pow(weight[i], 4)
				   * dot(corner_gradient[i],corner_distances2[i]);
		}
		
		return value;
	}
	double fbm(std::function<double(double,double)>f, double x, double y) {
		double sum = 0.0;
		double amp = 0.7;
		double freq = 0.05;

		for (int o = 0; o < 5; o++) {
			sum += amp * f(x * freq, y * freq);
			freq *= 2.0;
			amp  *= 0.5;
		}
		return sum;
	}
}
	

