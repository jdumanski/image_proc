#pragma once
#include <stdint.h>
#include <opencv2/opencv.hpp>

enum Image_type {
	PNG,
	JPEG
};

enum Pixel_type {
	RGB,
	Greyscale,
	RGBXYZ
};

template <typename pixelType>
class image_t { //image template class
	int width;
	int height;
	uint_8* bytes;
	Image_type imType;
public:
	image_t(int width, int height, char* bytes) : width(width), height(height), bytes(std::move(bytes)) {

	}

};
