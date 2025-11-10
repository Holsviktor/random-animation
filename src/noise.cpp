#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdint>

using std::vector;

#define SKEW_CONSTANT  0.366025403784 // (sqrt(3)-1)/2
#define INV_SKW_CONST  0.211324865405; // (3-sqrt(3))/6
		 
namespace noise {
	double simplex(int x, int y);
	std::pair<int,int> gridSkew(double x, double y);
	vector<int> permutationTable(uint64_t seed);
	vector<vector<double>> cornerDistances(double x, double y);
	inline double attenuate(double d) {return 1 - std::max(1/exp(d), 1.0);}

	static const int8_t gradents[8][2] = {
		{  1,  0 }, { -1,  0 }, {  0,  1 }, {  0, -1 },
		{  1,  1 }, { -1,  1 }, {  1, -1 }, { -1, -1 }
	};

	inline int gradIndex(int hash) {
		return hash & 7; // 8 gradients
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

	vector<vector<double>> cornerDistances(double x, double y) {

		// Find coordinates of upper left corner
		auto [i,j] = gridSkew(x,y);
		auto [corner_x, corner_y] = unskew(i,j);


		// Use that to find all corners
		double corner_x_offset = x - corner_x;
		double corner_y_offset = y - corner_y;
		int in_lower = (corner_x_offset > corner_y_offset);

		int corner_indexes[3][2] = {{i         ,j  },
							 		{i+1 ,j+1},
							 		{i+in_lower,j+!in_lower}};


		// Calculate distance to all corners
		vector<vector<double>> corner_distances(3,vector<double>(2,0.0));
		corner_distances[0][0] = corner_x_offset;
		corner_distances[0][1] = corner_y_offset;

		for (int i = 0; i < 3; i++) {
			auto [skewed_corner_x, skewed_corner_y] = corner_indexes[i];	
			auto [corner_x, corner_y] = unskew(skewed_corner_x, skewed_corner_y);
			corner_distances[i][0] = x - corner_x;
			corner_distances[i][1] = y - corner_y;
		}

		return corner_distances;
	}
}
	

