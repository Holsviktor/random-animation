// I/O
#include <iostream>

// Calculations
#include <vector>
#include <cstdint>

// Randomness
#include <chrono>
#include <random>

// Image generation
#include <opencv2/opencv.hpp>

#include "noise.hpp"

using std::vector;
using std::cout;

#define HEIGHT 1080
#define WIDTH  1920
#define CHANNEL_COUNT 3

#define DOT_SIZE 3
#define DOT_OFFSET 0
#define DOT_SPACE 10
#define GRID_SIZE (DOT_SIZE + DOT_SPACE)

#define OUTPUT_FILE "output_image.png"


namespace noise {
	int simplex();
}

int main() {

	// Initial setup
	auto time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937_64 random_engine = std::mt19937_64(time);
	
	vector<uint8_t> color_map = vector<uint8_t>(HEIGHT*WIDTH*CHANNEL_COUNT, 0);

	// Generate Grid
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			// For each pixel
			double x, y;
			x = i*0.01; y = j*0.01;

			vector<vector<double>> corner_offset_vectors = noise::cornerDistances(x,y);
			vector<double> corner_distances(3,0.0);

			for (int i = 0; i < 3; i++) {
				corner_distances[i] = std::sqrt(std::pow(corner_offset_vectors[i][0],2) + std::pow(corner_offset_vectors[i][1],2));
			}
			
			// Expand to all three channels
			int pixel_index = CHANNEL_COUNT * (i*WIDTH + j);
			color_map[pixel_index + 0] += corner_distances[0] < 0.3 ? 255 : 0;
			color_map[pixel_index + 1] += corner_distances[1] < 0.3 ? 255 : 0;
			color_map[pixel_index + 2] += corner_distances[2] < 0.3 ? 255 : 0;
		}
	}

	// Domain warp


	// Process image and write image to disk.
	cv::Mat img(HEIGHT, WIDTH, CV_8UC3, color_map.data());
	cv::imwrite(OUTPUT_FILE, img);

	cout << "Image written to " << OUTPUT_FILE << "\n";
}
