#pragma once
#include "Image.hpp"
#include "Point.hpp"
#include "Kernel.hpp"
#include <Eigen/Dense>

/*
For all image processing function, we specify the pixel type of the image being passed in
as this makes sense as not every function applies to every pixel type, as allows us to not have 
to templatize the ImageProcessing class

Also this is a static class (just a library)
*/


//list of kernels

// ALSO must apply normalizing factor within kernel as its not applied in ApplyKernel()
/*
so
a b c
d e f
g h i

corresponds to indices

0 1 2
3 4 5
6 7 8

*/

/*
const std::vector<std::vector<double>> CENTERDIFFX = { {1 / 12.0, -8 / 12.0, 0, 8 / 12.0, -1 / 12.0} };
const std::vector<std::vector<double>> CENTERDIFFY = {
	{1 / 12.0}, 
	{-8 / 12.0}, 
	{0}, 
	{8 / 12.0}, 
	{-1 / 12.0} };
const std::vector<std::vector<double>> FORWARDDIFFX = { {-3 / 2.0, 4 / 2.0, -1 / 2.0} };
const std::vector<std::vector<double>> FORWARDDIFFY = {
	{-3 / 2.0},
	{4 / 2.0},
	{-1 / 2.0} };
const std::vector<std::vector<double>> BACKDIFFX = { {1 / 2.0, -4 / 2.0, 3 / 2.0} };
const std::vector<std::vector<double>> BACKDIFFY = {
	{1 / 2.0},
	{-4 / 2.0},
	{3 / 2.0} };
*/

const Kernel SOBELX = {
	1, 0, -1,
	2, 0, -2,
	1, 0, -1
};
const Kernel SOBELY = {
	1, 2, 1,
	0, 0, 0,
	-1, -2, -1
};

const Kernel GAUSSBLUR3 = {
	1/16.0, 2/16.0, 1/16.0,
	2/16.0, 4/16.0, 2/16.0,
	1/16.0, 2/16.0, 1/16.0
};

const Kernel GAUSSBLUR5 = {
	1 / 256.0, 4 / 256.0, 6 / 256.0, 4 / 256.0, 1 / 256.0,
	4 / 256.0, 16 / 256.0, 24 / 256.0, 16 / 256.0, 4 / 256.0,
	6 / 256.0, 24 / 256.0, 36 / 256.0, 24 / 256.0, 6 / 256.0,
	4 / 256.0, 16 / 256.0, 24 / 256.0, 16 / 256.0, 4 / 256.0,
	1 / 256.0, 4 / 256.0, 6 / 256.0, 4 / 256.0, 1 / 256.0
};

class ImageProcessing {
	static const int EDGE_DETECTION_THRESH = 75;
	static const int KERNEL_RADIUS = 1;
	static const int LPF_THRESH = 57;

public:
	// converting functions:
	static void BGR24ToGrayscale24(Image<BGR>& im);
	static Image<Grayscale> BGR24ToGrayscale8(const Image<BGR>& im);
	static Image<BGR> Grayscale8ToGrayscale24(const Image<BGR>& im);

	// convolution functions:
	static void gaussianBlur(Image<BGR> const & src, Image<BGR>& dest);
	static void EdgeDetection(Image<BGR>& im, int thresh);
	static void EdgeDetectionSobel(Image<BGR>& im, int thresh, bool fourier);
	static void GaussianBlur3(Image<BGR>& im); 
	static void GaussianBlur5(Image<BGR>& im);
	static void Threshold(Image<BGR>& im, int thresh);

private:
	// disable constructor to make static class
	ImageProcessing() {}
	// helper functions
	static Pixel<BGR> ApplyKernel(const std::vector<std::vector<double>>& kernel, Point centerG, Point dims, const std::vector<Pixel<BGR>>& imData);


// -------------------------------------------------------- not used anymore
public:
	//LTIR /////////////////////
	static void Convolution(Image<BGR>& im, std::function<Pixel<BGR>(Point, Point, const std::vector<Pixel<BGR>>&)> kernel);
	static Pixel<BGR> EdgeDetectionKernel(Point currPoint, Point dims, const std::vector<Pixel<BGR>>& imData);
	static Pixel<BGR> LowPassFilterKernel(int x, int y, int height, int width, const std::vector<Pixel<BGR>>& imData);

private:
	static double partialX(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims);
	static double partialY(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims);
	static double centeredDiff(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims, bool isX);
	static double forwardDiff(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims, bool isX);
	static double backwardDiff(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims, bool isX);
};