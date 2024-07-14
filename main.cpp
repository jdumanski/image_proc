#include <iostream>
#include "Image.hpp"
#include "Pixel.hpp"
#include <string.h>
#include "ImageProcessing.hpp"
#include "Kernel.hpp"
#include "Bitmap.hpp"

// TODO
// refactor current code before moving on to freq domain stuff

int main(){
	BitmapReader reader;	
	std::cout << "Reading..." << std::endl;
	std::unique_ptr<Image<BGR>> img = nullptr;
	reader.readBMP<BGR>("IMAGES/blackbuck.bmp", img);

	BitmapWriter writer;
	std::cout << "Writing..." << std::endl;
	writer.writeBMP<BGR>("IMAGES/blackbuck_copy.bmp", *img);
	return 0;

	//Kernel k = { 1, 2, 3, 4 };

	//std::cout << k.getWeight(1, 1);

	/*
	1. create bitmap object
	2. pass in bitmap and extract data
	3. put data 
	*/

	//std::string path = "blackbuck.bmp";
	//std::string path2 = "LUG2.bmp";
	//std::string path3 = "field.bmp";
	//std::string path4 = "factory.bmp";

	//Image<BGR> im(path);
	//Image<BGR> im2(path);
	//Image<BGR> im3(path4);

	//std::unique_ptr<Image<PixelType::Grayscale>> imG = ImageProcessing::grayScale1(im);
	//ImageProcessing::BGR24ToGrayscale8(im3);
	//ImageProcessing::GaussianBlur5(im3);
	//ImageProcessing::EdgeDetectionSobel(im3, 100);
	//std::vector<Pixel<PixelType::BGR>> copy = im.getData();
	//ImageProcessing::Convolution(im, ImageProcessing::EdgeDetectionKernel);
	//ImageProcessing::EdgeDetection(im, 40);
	//ImageProcessing::grayScale3(im2);
	//ImageProcessing::grayScale3(im);
	//ImageProcessing::GaussianBlur3(im);
	//ImageProcessing::EdgeDetectionSobel(im, 300);
	//ImageProcessing::GaussianBlur5(im);
	//ImageProcessing::Threshold(im, 150);
	//ImageProcessing::EdgeDetection(im2, 45);

	/*
	std::vector<std::vector<double>> kernel = { {1/2.0, 0, 0} };
	Point centerG(1, 1);
	Point centerK(1, 0);
	Point dims(3, 3);
	std::vector<Pixel<BGR>> tempData = im.getData();
	im.setPixel(1, 1, ImageProcessing::ApplyKernel(kernel, centerK, centerG, dims, tempData));
	*/

	//im.setPixel(1, 1, im.getPixel(1, 1).scale(0.1));
	//im.BMPImageWrite("blackbuck_edgesFinal.bmp");
	//Image<Grayscale> imGray = ImageProcessing::grayScale1(im);
	//imGray.BMPImageWrite("blackbuck_gray.bmp");
	//im.BMPImageWrite("blackbuck_egdesRelease.bmp");
	//im3.BMPImageWrite("factory_edges_2.bmp");

	//imG->BMPImageWrite("large_g.bmp");
}