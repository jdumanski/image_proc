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
	
	static const int fileHeaderSize = 14;
	int informationHeaderSize = 40;
	uint8_t fileHeader[fileHeaderSize];
	std::vector<uint8_t> informationHeader;
	
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

		informationHeader.resize(informationHeaderSize);

		if (fileHeader[0] == 'B' && fileHeader[1] == 'M') {
			BMPImageRead(f);
		}
		f.close();
		//do conditions for other file types
	}

	
	Image(int width, int height, std::vector<Pixel<p>>& data, int informationHeaderSize=40) : width(width), height(height), data(data) {
		if (data.size() != width * height) {
			throw std::runtime_error("width and height do not match data vector size");
		}
		informationHeader.resize(informationHeaderSize);
	}

	bool BMPImageRead(std::ifstream& f) {
		if (!f.is_open()) {
			std::cout << "file could not be opened" << std::endl;
			return false;
		}

		int defaultInfoHeaderSize = informationHeaderSize; //default is 40 (in class definition)

		//read first 40 bytes of info header
		f.read(reinterpret_cast<char*>(informationHeader.data()), informationHeaderSize);
		informationHeaderSize = informationHeader[0];

		//read remaining bytes in info header
		informationHeader.resize(informationHeaderSize);
		f.read(reinterpret_cast<char*>(informationHeader.data() + sizeof(uint8_t) * defaultInfoHeaderSize), informationHeaderSize - defaultInfoHeaderSize);

		//bit shift to get each byte of the int (which is fileSize) and then add then together - need to bit shift to get decimal places correct
		int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
		width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
		height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);
		
		std::cout << "Info header size is " << (int)informationHeader[0] << " bytes" << std::endl;;

		data.resize(width * height);

		int paddingAmount = (4 - (width * p) % 4) % 4; //recall p is number of channels
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				uint8_t buffer[p];
				f.read(reinterpret_cast<char*>(buffer), p); //read pixel data into buffer
				for (int channel = 0; channel < p; channel++) { //put pixel data from buffer to data vector
					data[y * width + x][channel] = buffer[channel]; //note colours are stored BGR
				}
			}
			f.ignore(paddingAmount);
		}
		std::cout << "file read!" << std::endl;
		return true;
	}

	//creates a new file of the image
	bool BMPImageWrite(std::string path) {
		std::ofstream f;		
		f.open(path, std::ios::out | std::ios::binary);
		if (!f.is_open()) {
			std::cout << "file could not be opened/created" << std::endl;
			return false;
		}

		uint8_t bmpPad[3] = {0, 0, 0};
		const int paddingAmount = (4 - (width * p) % 4) % 4;

		const int fileSize = fileHeaderSize + informationHeaderSize + width * height * p + paddingAmount * height;

		//file type
		fileHeader[0] = 'B';
		fileHeader[1] = 'M';
		//file size
		fileHeader[2] = fileSize;
		fileHeader[3] = fileSize >> 8;
		fileHeader[4] = fileSize >> 16;
		fileHeader[5] = fileSize >> 24;
		//reserved
		fileHeader[6] = 0;
		fileHeader[7] = 0;
		//reserved
		fileHeader[8] = 0;
		fileHeader[9] = 0;
		//pixel data offset
		fileHeader[10] = fileHeaderSize + informationHeaderSize;
		fileHeader[11] = 0;
		fileHeader[12] = 0;
		fileHeader[13] = 0;

		//header size
		informationHeader[0] = informationHeaderSize;
		informationHeader[1] = 0;
		informationHeader[2] = 0;
		informationHeader[3] = 0;
		//image width
		informationHeader[4] = width;
		informationHeader[5] = width >> 8;
		informationHeader[6] = width >> 16;
		informationHeader[7] = width >> 24;
		//image height
		informationHeader[8] = height;
		informationHeader[9] = height >> 8;
		informationHeader[10] = height >> 16;
		informationHeader[11] = height >> 24;
		//Planes
		informationHeader[12] = 1;
		informationHeader[13] = 0;
		//bits per pixel
		informationHeader[14] = 8*p;
		informationHeader[15] = 0;
		//compression
		informationHeader[16] = 0;
		informationHeader[17] = 0;
		informationHeader[18] = 0;
		informationHeader[19] = 0;
		//image size
		informationHeader[20] = 0;
		informationHeader[21] = 0;
		informationHeader[22] = 0;
		informationHeader[23] = 0;
		//x pixels per meter
		informationHeader[24] = 0;
		informationHeader[25] = 0;
		informationHeader[26] = 0;
		informationHeader[27] = 0;
		//y pixels per meter
		informationHeader[28] = 0;
		informationHeader[29] = 0;
		informationHeader[30] = 0;
		informationHeader[31] = 0;
		//total colours
		informationHeader[32] = 0;
		informationHeader[33] = 0;
		informationHeader[34] = 0;
		informationHeader[35] = 0;
		//important colours
		informationHeader[36] = 0;
		informationHeader[37] = 0;
		informationHeader[38] = 0;
		informationHeader[39] = 0;

		//remaining informationHeader values over index 39 are default set to zero when informationHeader array is resized over 40

		f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
		f.write(reinterpret_cast<char*>(informationHeader.data()), informationHeaderSize);

		for (int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				//need to transfer into uint8_t container bc pixel stores channels as ints, so it can do operations;
				uint8_t tempData[p];
				int* dataArr = this->getPixel(x, y).data;
				for (int i = 0; i < p; i++) {
					tempData[i] = dataArr[i];
				}
				f.write(reinterpret_cast<char*>(tempData), p);
			}
			f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
		}
		f.close();
		std::cout << "file wrote to successfully" << std::endl;
		return true;
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

	//can modify pixel (returns ref)
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