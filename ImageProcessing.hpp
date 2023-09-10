#pragma once
#include "Image.hpp"
#include "Point.hpp"
#include <Eigen/Dense>

/*
For all image processing function, we specify the pixel type of the image being passed in
as this makes sense as not every function applies to every pixel type, as allows us to not have 
to templatize the ImageProcessing class

Also this is a static class (just a library)
*/
class ImageProcessing {
	static const int EDGE_DETECTION_THRESH = 75;
	static const int KERNEL_RADIUS = 1;
	static const int LPF_THRESH = 57;
public:
	static void grayScale3(Image<BGR>& im);
	static std::unique_ptr<Image<Grayscale>> grayScale1(const Image<BGR>& im);
	static void EdgeDetection(Image<BGR>& im, int thresh);
	static void LowPassFilter(Image<BGR>& im, int kernelDim); //
	static Pixel<BGR> ApplyKernel(std::vector<std::vector<double>> kernel, Point centerK, Point centerG, Point dims, const std::vector<Pixel<BGR>>& imData);


	// LTIR /////////////////////
	static void Convolution(Image<BGR>& im, std::function<Pixel<BGR>(Point, Point, const std::vector<Pixel<BGR>>&)> kernel);
	static Pixel<BGR> EdgeDetectionKernel(Point currPoint, Point dims, const std::vector<Pixel<BGR>>& imData);
	static Pixel<BGR> LowPassFilterKernel(int x, int y, int height, int width, const std::vector<Pixel<BGR>>& imData);


private:
	ImageProcessing() {} //disable constructor to make static class
	static double partialX(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims);
	static double partialY(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims);
	static double centeredDiff(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims, bool isX);
	static double forwardDiff(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims, bool isX);
	static double backwardDiff(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims, bool isX);
};