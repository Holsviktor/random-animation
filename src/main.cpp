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

#define SCALE 0.01
#define DOT_SIZE 1 //*SCALE
#define DOT_OFFSET 0 //*SCALE
#define DOT_SPACE 3 //*SCALE
#define GRID_SIZE (DOT_SIZE + DOT_SPACE)

#define OUTPUT_FILE "output_image.png"

vector<double> color_map = vector<double>(HEIGHT*WIDTH*CHANNEL_COUNT, 0);

double gridcolor(double x, double y) {
	return 255.0 
		* (std::fmod(x + DOT_OFFSET,GRID_SIZE) < DOT_SIZE)
		* (std::fmod(y + DOT_OFFSET,GRID_SIZE) < DOT_SIZE);
}

void save_image(int i) {
	// Process image and write image to disk.
	std::string filename = "images/" + std::to_string(i) + ".png";
	cv::Mat scaled_image(HEIGHT, WIDTH, CV_64FC3, color_map.data());
	cv::Mat img;
	cv::normalize(scaled_image, img, 0, 255, cv::NORM_MINMAX, CV_8UC3);
	cv::imwrite(filename, img);
	cout << "Image written to " << filename << "\n";

}
void generate_grid(double t, int i) {
	std::fill(color_map.begin(), color_map.end(), 0.0);
	for     (double i = 20; i < -50 + (double) WIDTH  / GRID_SIZE; i++) {
		for (double j = 20; j < -20 + (double) HEIGHT / GRID_SIZE; j++) {

			double x = SCALE * (i * GRID_SIZE + DOT_OFFSET);
			double y = SCALE * (j * GRID_SIZE + DOT_OFFSET);

			double warped_x = (x + 150.0*noise::fbm(noise::simplex, x+0.5*t     , y     )) / SCALE;
			double warped_y = (y + 150.0*noise::fbm(noise::simplex, x+10.0, y+10.0+0.5*t)) / SCALE;

			for (int _i = warped_x; _i <= warped_x; _i++) {
				for (int _j = warped_y; _j <= warped_y; _j++) {
					int pixel_index = CHANNEL_COUNT *(_j * WIDTH + _i);

					if (pixel_index < 0 || pixel_index > CHANNEL_COUNT*HEIGHT*WIDTH-3) {
						continue;
					}
					if ( warped_x < 0 || warped_x > WIDTH
					  || warped_y < 0 || warped_y > HEIGHT)
						continue;

					color_map[pixel_index+0] = 255.0;
					color_map[pixel_index+1] = 255.0;
					color_map[pixel_index+2] = 255.0;
				}
				
			}
		}
	}
	save_image(i);
	
}
void generate_fog(double t, int i) {
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			// For each pixel
			double x, y;
			x = i*SCALE ; y = j*SCALE;
			double brightness = 0.0;
			double xwarp, ywarp;

			xwarp       = noise::fbm(noise::simplex, x+0.5*t      , y      );
			ywarp       = noise::fbm(noise::simplex, x+100.0, y+100.0+0.5*t);

			xwarp = 50.0*(std::fabs(xwarp));
			ywarp = 50.0*(std::fabs(ywarp));
			brightness  = noise::fbm(noise::simplex, x+xwarp, y+ywarp); //gridcolor(x+xwarp, y+ywarp);
			
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
	save_image(i);

}
int main() {

	// Initial setup
	auto time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937_64 random_engine = std::mt19937_64(time);
	

	// Generate Grid
	noise::initPermutationTable(1337);

	for (int i = 0; i < 100; i++) {
		generate_fog((double) i / 10.0, i);
	}


}
