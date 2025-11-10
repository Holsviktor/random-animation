#include <iostream>
#include <vector>
#include <cstdint>
#include <opencv2/opencv.hpp>

using std::vector;
using std::cout;

#define HEIGHT 640
#define WIDTH  480
#define CHANNEL_COUNT 3

int main() {
	vector<uint8_t> color_map = vector<uint8_t>(HEIGHT*WIDTH*CHANNEL_COUNT, 0);

	int previous_color = 0;
	for (uint8_t& color : color_map) {
		color = (previous_color + 1) % 256;
		previous_color = color;
	}

	cv::Mat img(HEIGHT, WIDTH, CV_8UC3, color_map.data());
	
	cv::imwrite("output_image.png", img);
	
}
