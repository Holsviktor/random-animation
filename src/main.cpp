// I/O
#include <iostream>

// Calculations
#include <vector>
#include <cstdint>
#include <functional>

// Randomness
#include <chrono>
#include <random>

// Image generation
#include <opencv2/opencv.hpp>

#include "noise.hpp"

using std::vector;
using std::cout;

#define WIDTH  1920
#define HEIGHT 1080
#define CHANNEL_COUNT 3

#define SCALE 0.005
#define DOT_SIZE 3
#define DOT_OFFSET 0
#define DOT_SPACE 10
#define GRID_SIZE (DOT_SIZE + DOT_SPACE)

#define OUTPUT_FILE "output_image.png"

int main() {

	// Initial setup
	auto time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937_64 random_engine = std::mt19937_64(time);
	double max = -INFINITY;
	double min = INFINITY;
	
	vector<double> color_map = vector<double>(HEIGHT*WIDTH*CHANNEL_COUNT, 0);

	// Generate Grid
	noise::initPermutationTable(1337123212231343213);

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			// For each pixel
			double x, y;
			x = i*SCALE ; y = j*SCALE;
			double brightness = noise::fbm(noise::simplex, x,y);
			if (brightness > max) {
				max = brightness;
			}
			if (brightness < min) {
				min = brightness;
			}
			
			double blue, green, red;
			blue = green = red = brightness;
			for (int corner = 0; corner < 3; corner++) {
				// Expand to all three channels
				int pixel_index = CHANNEL_COUNT * (i*WIDTH + j);
				color_map[pixel_index + 0] += blue;
				color_map[pixel_index + 1] += green;
				color_map[pixel_index + 2] += red;
			}
		}
	}

	cout << "\n" << min << "  " << max << "\n";

	// Process image and write image to disk.
	cv::Mat scaled_image(HEIGHT, WIDTH, CV_64FC3, color_map.data());
	cv::Mat img;
	cv::normalize(scaled_image, img, 0, 255, cv::NORM_MINMAX, CV_8UC3);
	cv::imwrite(OUTPUT_FILE, img);

	cout << "Image written to " << OUTPUT_FILE << "\n";
}
