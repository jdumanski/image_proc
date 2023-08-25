#pragma once
#include <opencv2/opencv.hpp>
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

	static const int fileHeaderSize = 14;
	static const int informationHeaderSize = 40;

	uint8_t fileHeader[fileHeaderSize];
	uint8_t informationHeader[informationHeaderSize];

	//note channels implied through PixelType p
public:
	Image(std::string path) {

		std::ifstream f;
		f.open(path, std::ios::in | std::ios::binary);
		if (!f.is_open()) {
			std::cout << "file could not be opened" << std::endl;
			return;
		}
		std::cout << "file opened!" << std::endl;

		f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

		if (fileHeader[0] == 'B' && fileHeader[1] == 'M') {
			BMPImageRead(f);
		}
		f.close();
		//do conditions for other file types
	}
	bool BMPImageRead(std::ifstream& f) {
		if (!f.is_open()) {
			std::cout << "file could not be opened" << std::endl;
			return false;
		}
		
		f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

		//bit shift to get each byte of the int (which is fileSize) and then add then together - need to bit shift to get decimal places correct
		int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
		width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
		height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

		data.resize(width * height);

		const int paddingAmount = (4 - (width * p) % 4) % 4; //recall p is number of channels
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				uint8_t buffer[p];
				f.read(reinterpret_cast<char*>(buffer), p); //read pixel data into buffer
				for (int channel = 0; channel < p; channel++) { //put pixel data from buffer to data vector
					data[y * height + x][channel] = buffer[channel]; //note colours are stored BGR
				}
			}
			f.ignore(paddingAmount);
		}
		std::cout << "file read!" << std::endl;
		return true;
	}

	//bool BMPImageWrite

	Image(int width, int height, std::vector<Pixel<p>>& data) : width(width), height(height), data(data){}
	//disable copying ands assignment bc expensive operation
	Image(const Image&) = delete;
	Image& operator=(Image&) = delete;
	//x and y are zero indexed
	
	//can also make function that takes in a kernel, and applies it to every pixel in the image - this could be very handy
	bool setPixel(int x, int y, Pixel<p> val) {
		if (x < width && y < height) {
			int startIdx = y * width + x;
			for (int i = 0; i < p; i++) {
				data[startIdx][i] = val[i];
			}
			return true;
		}
		return false;
	}
	//cannot modify pixel - returned by const reference
	Pixel<p> getPixel(int x, int y) const {
		return data[y * width + x];
	}

	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
	//returns constant reference to data
	std::vector<Pixel<p>>& getData() const {
		return data;
	}


	
	

	
};