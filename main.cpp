#include <iostream>
#include "Image.hpp"
#include "Pixel.hpp"
#include <string.h>
#include "ImageProcessing.hpp"

int main() {
	/*
	std::vector<Pixel<BGR>> data;
	Pixel<PixelType::BGR> p1({ 1, 2, 3 });
	Pixel<PixelType::BGR> p2({ 4, 5, 6 });
	Pixel<PixelType::BGR> p3({ 7, 8, 9 });

	Pixel<PixelType::BGR> p4({ 10, 11, 12 });
	Pixel<PixelType::BGR> p5({ 13, 14, 15 });
	Pixel<PixelType::BGR> p6({ 16, 17, 18 });

	Pixel<PixelType::BGR> p7({ 19, 20, 21 });
	Pixel<PixelType::BGR> p8({ 22, 23, 24 });
	Pixel<PixelType::BGR> p9({ 25, 26, 27 });

	data.push_back(p1);
	data.push_back(p2);
	data.push_back(p3);
	data.push_back(p4);
	data.push_back(p5);
	data.push_back(p6);
	data.push_back(p7);
	data.push_back(p8);
	data.push_back(p9);


	Image<PixelType::BGR> im(3, 3, data);
	Pixel<PixelType::BGR> pix = im.getPixel(0, 2);
	std::cout << (int)pix[1] << std::endl;

	Pixel<PixelType::BGR> pix2({ 1, 2, 3 });

	im.setPixel(2, 2, pix2);
	std::cout << (int)data[8][1];
	*/

	/*
	1. create bitmap object
	2. pass in bitmap and extract data
	3. put data 
	*/

	std::string path = "large.bmp";
	Image<PixelType::BGR> im(path);
	ImageProcessing::grayScale(im);

	im.BMPImageWrite("large_g.bmp");
	
	

	


	return 0;
}