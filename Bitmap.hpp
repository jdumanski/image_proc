#pragma once
#include "Image.hpp"
#include <string.h>
#include <stdint.h>
#include <iostream>
//#include <memory>
#include <fstream>

// static helper class for reading/writing to bitmap images

/*
* Want to templatize because need pixel type (either in template or as function parameter) to know how to interperet channels of BMP image
* But since image is templatized, and we want to return an Image, we should templatize BitmapReader and BitmapWriter
*/

class BitmapReader{
	static const int fileHeaderSize = 14; // double check this is always 14 for bit maps
	int informationHeaderSize = 40;
	uint8_t fileHeader[fileHeaderSize];
	std::vector<uint8_t> informationHeader;

public:
// return image instead of passing one in by ref, because can only set data, width, and height in constructor, so we make our own inside readBMP
	template <PixelType p>
	bool readBMP(std::string file_path, std::unique_ptr<Image<p>> & src) {
		std::ifstream f;
		f.open(file_path, std::ios::in | std::ios::binary);
		if (!f.is_open()) {
			std::cout << "file could not be opened" << std::endl;
			return false;
		}
		std::cout << "file opened!" << std::endl;

		f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

		informationHeader.resize(informationHeaderSize);

		if (fileHeader[0] == 'B' && fileHeader[1] == 'M') {
			//const int pixel_alloc_size = std::max(2, p); // cant be less than 2, bc each pixel must allocate min of 16 bits

			int defaultInfoHeaderSize = informationHeaderSize; //default is 40 (in class definition)

			//read first 40 bytes of info header
			f.read(reinterpret_cast<char*>(informationHeader.data()), informationHeaderSize);
			informationHeaderSize = informationHeader[0];

			//read remaining bytes in info header
			informationHeader.resize(informationHeaderSize);
			f.read(reinterpret_cast<char*>(informationHeader.data() + sizeof(uint8_t) * defaultInfoHeaderSize), informationHeaderSize - defaultInfoHeaderSize);

			//bit shift to get each byte of the int (which is fileSize) and then add then together - need to bit shift to get decimal places correct
			int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
			int width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
			int height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

			std::cout << "Info header size is " << (int)informationHeader[0] << " bytes" << std::endl;;

			std::vector<Pixel<p>> data(width * height);
			//data.resize(width * height);

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
			f.close();
			src = std::make_unique<Image<p>>(width, height, data);
			std::cout << "file read!" << std::endl;
		}
		else {
			f.close();
			std::cout << "Not BMP file" << std::endl;
			src = nullptr;
			return false;
		}
		return true;
	}
};

class BitmapWriter {
	static const int fileHeaderSize = 14;
	int informationHeaderSize = 40;
	uint8_t fileHeader[fileHeaderSize];
	std::vector<uint8_t> informationHeader;

public:
	template <PixelType p>
	bool writeBMP(std::string file_path, const Image<p> &src) {
		int width = src.getWidth();
		int height = src.getHeight();
		std::ofstream f;
		f.open(file_path, std::ios::out | std::ios::binary);
		if (!f.is_open()) {
			std::cout << "file could not be opened/created" << std::endl;
			return false;
		}
		std::cout << informationHeaderSize << std::endl;
		uint8_t bmpPad[3] = { 0, 0, 0 };
		const int paddingAmount = (4 - (width * p) % 4) % 4;

		const int fileSize = fileHeaderSize + informationHeaderSize + width * height * p + paddingAmount * height;

		informationHeader.resize(informationHeaderSize);

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
		informationHeader[14] = 8 * p;
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
			for (int x = 0; x < width; x++) {
				//need to transfer into uint8_t container bc pixel stores channels as ints, so it can do operations;
				uint8_t tempData[p];
				int* dataArr = src.getPixel(x, y).data;
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
};
