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
			auto [tile_i, tile_j] = noise::tileIndex(i*0.01, j*0.01);
			int color = (tile_i + tile_j) & 3;

			// Expand to all three channels
			int pixel_index = CHANNEL_COUNT * (i*WIDTH + j);
			switch (color) {
				case 0:
					color_map[pixel_index + 0] = 200;
					color_map[pixel_index + 1] = 200;
					color_map[pixel_index + 2] = 200;
					break;
				case 1:
					color_map[pixel_index + 0] = 200;
					color_map[pixel_index + 1] = 100;
					color_map[pixel_index + 2] = 100;
					break;
				case 2:
					color_map[pixel_index + 0] = 100;
					color_map[pixel_index + 1] = 200;
					color_map[pixel_index + 2] = 100;
					break;
				case 3:
					color_map[pixel_index + 0] = 100;
					color_map[pixel_index + 1] = 100;
					color_map[pixel_index + 2] = 200;
					break;
			}
		}
	}

	// Domain warp


	// Process image and write image to disk.
	cv::Mat img(HEIGHT, WIDTH, CV_8UC3, color_map.data());
	cv::imwrite(OUTPUT_FILE, img);

	cout << "Image written to " << OUTPUT_FILE << "\n";
}
