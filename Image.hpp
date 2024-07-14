#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/core.hpp>
#include "Pixel.hpp"
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <fstream>

template <PixelType p>
class Image { //image template class
	int width;
	int height;
	std::vector<Pixel<p>> data;

	// note channels implied through PixelType p
public:

	Image(int width, int height, std::vector<Pixel<p>>& data) : width(width), height(height), data(data) {
		if (data.size() != width * height) {
			throw std::runtime_error("width and height do not match data vector size");
		}
	}
	
	//disable copying and assignment bc expensive operation
	//Image(const Image&) = delete;
	Image& operator=(Image&) = delete;
	//x and y are zero indexed
	
	//can also make function that takes in a kernel, and applies it to every pixel in the image - this could be very handy
	bool setPixel(int x, int y, Pixel<p> val) {
		if (x < width && y < height) {
			int startIdx = y * width + x;
			for (int i = 0; i < p; i++) {
				if (val[i] > 255) {
					val[i] = 255;
				}
				data[startIdx][i] = val[i];
			}
			return true;
		}
		return false;
	}
	//cannot modify pixel
	Pixel<p> getPixel(int x, int y) const {
		return data[y * width + x];
	}

	//can modify pixel (returns reference)
	Pixel<p>& getPixel(int x, int y) {
		return data[y * width + x];
	}

	inline int getWidth() const {
		return width;
	}
	inline int getHeight() const {
		return height;
	}
	//returns constant image pixel data - need in order to use copy of image data to process images whose kernel depends on surroundings pixels
	//this is the only time that we "copy" an image - dont want to copy image in most cases
	std::vector<Pixel<p>> getData() const {
		return data;
	}

	//want to modify image, or make new image during image processing? if modify image, must allow copying of data, but if making new image dont need to make copy, and can return with std::move - making new image
	//basically just as expensive, but is good bc we can completely eliminate posibility of copying
	//BUTTTT if we use modify image, even tho copying will be possible, copy of data is local to the processing function do it will be destroyed at end of function anyway
	//also makes more sense to keep modifying one image, as opposed to making 10 new images if we process image 10 times.
};