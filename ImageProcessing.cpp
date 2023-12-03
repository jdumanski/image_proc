#include "ImageProcessing.hpp"


//TODO
/*
1. 24 to 8 and 8 to 24 - done
2. get rgb -> single channel grayscale working for exporting - low priority
3. change from editing existing image object, to creating new image object
4. deal with edge pixels for kernels
5. implement canny edge detection
6. implement faster convolution
7. use multithreading for convolution

*/

//converts rbg image to 3 channel grayscale image
void ImageProcessing::BGR24ToGrayscale24(Image<BGR>& im) {
	for (int y = 0; y < im.getHeight(); y++) {
		for (int x = 0; x < im.getWidth(); x++) {
			Pixel<BGR> pix = im.getPixel(x, y); //getpixel() returns by ref so we can modify pixel directly
			uint8_t grayVal = round(pix[2]*0.2989+pix[1]*0.5870+pix[2]*0.1140); //no need to worry about overflow since we are averging uint8_t values
			Pixel<BGR> grayPix({ grayVal, grayVal, grayVal });
			im.setPixel(x, y, grayPix);
		}
		//std::cout << "done row " << y << std::endl;
	}
}

Image<BGR> Grayscale8ToGrayscale24(const Image<Grayscale>& im) {
	std::vector<Pixel<BGR>> grayData;
	grayData.resize(im.getHeight() * im.getWidth());
	for (int y = 0; y < im.getHeight(); y++) {
		for (int x = 0; x < im.getWidth(); x++) {
			Pixel<Grayscale> pix = im.getPixel(x, y);
			uint8_t val = pix[0];
			Pixel<BGR> grayPix({ val, val, val });
			grayData[y * im.getWidth() + x] = grayPix;
		}
	}
	Image<BGR> imGray(im.getWidth(), im.getHeight(), grayData);
	return imGray;
}

//creates new single channel grayscale image from rgb image
Image<Grayscale> ImageProcessing::BGR24ToGrayscale8(const Image<BGR>& im) {
	std::vector<Pixel<Grayscale>> grayData;
	grayData.resize(im.getHeight() * im.getWidth());
	for (int y = 0; y < im.getHeight(); y++) {
		for (int x = 0; x < im.getWidth(); x++) {
			Pixel<BGR> pix = im.getPixel(x, y);
			grayData[y * im.getWidth() + x][0] = round((pix[0] * 0.2989 + pix[1] * 0.5870 + pix[2] * 0.1140));
		}
	}
	Image<Grayscale> imGray(im.getWidth(), im.getHeight(), grayData);
	return imGray; // dont need std::move because most complilers make lvalues eligible for NRVO (named (lvalue) returned value optimization), ie compiler
	//will do move for you, and that move is more efficient than std::move
}

//simple edge detection using derivatives?
//takes in image and threshold, and detects edges above gradient threshold
 //once this works, make general kernel function that takes in a kernel function, and iterates overall element in the image with that kernel
 //takes in kernel and image, makes copy of image data, then iterates over all pixels in data and applied kernel to copied data, but sets final value (output of kernel) to actual image
 void ImageProcessing::EdgeDetection(Image<BGR>& im, int thresh) {
	 Pixel<BGR> whitePix({ 255, 255, 255 });
	 Pixel<BGR> blackPix({ 0, 0, 0 });
	 Point dims(im.getWidth(), im.getHeight());
	 std::vector<Pixel<BGR>> dataCopy = im.getData();
	 for (int y = 0; y < dims.y; y++) {
		 for (int x = 0; x < dims.x; x++) {
			 Point currPoint(x, y);
			 double pX = partialX(dataCopy, currPoint, dims);
			 double pY = partialY(dataCopy, currPoint, dims);
			 double gradientMag = sqrt(pX*pX + pY*pY);
			 if (gradientMag > thresh) {
				 im.setPixel(x, y, whitePix);
			 }
			 else {
				 im.setPixel(x, y, blackPix);
			 } 
			 
			 //Pixel<BGR> gradientPix({ (uint8_t)gradientMag, (uint8_t)gradientMag, (uint8_t)gradientMag });
			 //im.setPixel(x, y, gradientPix);
		 }
		 //std::cout << "row " << y << " done" << std::endl;
	 }
 }

 void ImageProcessing::EdgeDetectionSobel(Image<BGR>& im, int thresh) {
	 Pixel<BGR> whitePix({ 255, 255, 255 });
	 Pixel<BGR> blackPix({ 0, 0, 0 });
	 Point dims(im.getWidth(), im.getHeight());
	 std::vector<Pixel<BGR>> dataCopy = im.getData();
	 Point centerK(1, 1);

	 for (int y = 0; y < dims.y; y++) {
		 for (int x = 0; x < dims.x; x++) {
			 Point centerG(x, y);
			 //gradient in X
			 double Gx = ApplyKernel(SOBELX, centerK, centerG, dims, dataCopy).norm();
			 double Gy = ApplyKernel(SOBELY, centerK, centerG, dims, dataCopy).norm();
			 double gradientMag = sqrt(Gx * Gx + Gy * Gy);
			 if (gradientMag > thresh) {
				 im.setPixel(x, y, whitePix);
			 }
			 else {
				 im.setPixel(x, y, blackPix);
			 }
			 
		 }
		 if(y%100==0)
			std::cout << "on row " << y << std::endl;
	 }
 }

 //kernel dim must be odd - if even we make it odd
 void ImageProcessing::GaussianBlur3(Image<BGR>& im) {
	 Point dims(im.getWidth(), im.getHeight());
	 std::vector<Pixel<BGR>> dataCopy = im.getData();
	 Point centerK(1, 1);
	 for (int y = 0; y < dims.y; y++) {
		 for (int x = 0; x < dims.x; x++) {
			 Point centerG(x, y);
			 im.setPixel(x, y, ApplyKernel(GAUSSBLUR3, centerK, centerG, dims, dataCopy));
		 }
		 std::cout << "on row " << y << std::endl;
	 }
 }

 void ImageProcessing::GaussianBlur5(Image<BGR>& im) {
	 Point dims(im.getWidth(), im.getHeight());
	 std::vector<Pixel<BGR>> dataCopy = im.getData();
	 Point centerK(2, 2);
	 for (int y = 0; y < dims.y; y++) {
		 for (int x = 0; x < dims.x; x++) {
			 Point centerG(x, y);
			 im.setPixel(x, y, ApplyKernel(GAUSSBLUR5, centerK, centerG, dims, dataCopy));
			 
		 }
		 std::cout << "on row " << y << std::endl;
	 }
 }

 void ImageProcessing::Threshold(Image<BGR>& im, int thresh) {
	 Point dims(im.getWidth(), im.getHeight());
	 Pixel<BGR> whitePix({ 255, 255, 255 });
	 Pixel<BGR> blackPix({ 0, 0, 0 });
	 for (int y = 0; y < dims.y; y++) {
		 for (int x = 0; x < dims.x; x++) {
			 Pixel<BGR> pix = im.getPixel(x, y); //getpixel() returns by ref so we can modify pixel directly
			 uint8_t avg = round(pix.average()); //no need to worry about overflow since we are averging uint8_t values
			 if (avg > thresh) {
				 im.setPixel(x, y, whitePix);
			 }
			 else {
				 im.setPixel(x, y, blackPix);
			 }
		 }
	 }
 }



 //centerK is coordiante of center in kernel space (reference frame of the kernel) - its zero indexed
 //even though vector of vectors, assumed to be RECTANGULAR (ie all inside vectors are same size)
 //can make "non square" kernel by setting kernel index values to zero
 //for kernel inside vectors are rows
 //k is for kernel space, g is for global space
 //kernelSize isthe 
 Pixel<BGR> ImageProcessing::ApplyKernel(const std::vector<std::vector<double>>& kernel, Point centerK, Point centerG, Point dims, const std::vector<Pixel<BGR>>& imData) {
	 int kernelWidth = kernel[0].size();
	 int kernelHeight = kernel.size();
	 Point transform = centerG - centerK;
	 Pixel<BGR> convSum({ 0, 0, 0 });
	 std::vector<Point> skippedIndices;
	 for (int xk = 0; xk < kernelWidth; xk++) {
		 for (int yk = kernelHeight - 1; yk >= 0; yk--) {
			 Point kernCoord(xk, yk);
			 Point imCoord = kernCoord + transform;
			 if (imCoord.x < 0 || imCoord.y < 0 || imCoord.x >= dims.x || imCoord.y >= dims.y) {
				 //handle if kernel goes beyond image
				 continue;
			 }
			 Pixel<BGR> imPixel = imData[imCoord.y * dims.x + imCoord.x];
			 Pixel<BGR> convPixel = imPixel.scale(kernel[yk][xk]); //kernel value at (xk, yk) multiplied on image pixel
			 convSum += convPixel;
		 }
	 }
	 return convSum;
 }

 //calculares partial derivative of a pixel wrt x by summing partial derivatives of r, g, and b channels wrt x
 double ImageProcessing::partialX(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims) {
	 if (currPoint.x < 2) {
		 return forwardDiff(data, currPoint, dims, 1);
	 }
	 else if (currPoint.x > dims.x - 3) {
		 return backwardDiff(data, currPoint, dims, 1);
	 }
	 return centeredDiff(data, currPoint, dims, 1);
 }

 //calculates partial derivative of a pixel wrt y by summing partial derivatives of r, g, and b channels wrt y
 double ImageProcessing::partialY(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims) {
	 if (currPoint.y < 2) {
		 return forwardDiff(data, currPoint, dims, 0);
	 }
	 else if (currPoint.y > dims.y - 3) {
		 return backwardDiff(data, currPoint, dims, 0);
	 }
	 return centeredDiff(data, currPoint, dims, 0);
 }

 //calculates centered difference derivative of r, g, and b streams, and returns the magnitude of these
 //idx to index of pixel to calcualte derivative about
 //dataIncrement is the number of vector indices between adjacent function values (image pixels) used in the numerical calculation - 1 for x and width for y, since moving in the x direction is 
 //equivalent to moving one index, and moving in the y direction is the equivalent to moving 'width' indicies in the vector
 double ImageProcessing::centeredDiff(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims, bool isX) {
	 /*
	 std::vector<std::vector<double>> kernel = { {1/12.0, -8/12.0, 0, 8/12.0, -1/12.0} };
	 Point centerK(2, 0); //in x - y coordinates, now row, col
	 if (!isX) {
		 kernel = { {1/12.0},
					{-8/12.0},
					{0},
					{8/12.0},
					{-1/12.0} };
		 centerK.x = 0;
		 centerK.y = 2;
	 }
	 Pixel<BGR> partialPix = ApplyKernel(kernel, centerK, currPoint, dims, data);
	 return partialPix.norm();



	 Pixel<BGR> partialPix;
	 Point centerK(2, 0);
	 if (!isX) {
		 centerK.x = 0;
		 centerK.y = 2;
		 partialPix = ApplyKernel(CENTERDIFFY, centerK, currPoint, dims, data);
	 }
	 else {
		 partialPix = ApplyKernel(CENTERDIFFX, centerK, currPoint, dims, data);
	 }
	 return partialPix.norm();
	 */
	 int step = 1;
	 if (!isX) {
		 step = dims.x;
	 }
	 int idx = currPoint.y * dims.x + currPoint.x;
	 Pixel<BGR> partialPix = (data[idx + 2 * step].scale(-1) + data[idx + 1 * step].scale(8) + data[idx - 1 * step].scale(-8) + data[idx - 2 * step]).scale(1 / 12.0);
	 return partialPix.norm();
 }

 //same role as centered diff but used for pixels that are on or one pixel away from the bottom or left borders
 double	ImageProcessing::forwardDiff(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims, bool isX) {
	 /*
	 std::vector<std::vector<double>> kernel = { {-3/2.0, 4/2.0, -1/2.0} };
	 Point centerK(0, 0);
	 if (!isX) {
		 kernel = { {-3 / 2.0},
					{4 / 2.0},
					{-1 / 2.0} };
		 centerK.x = 0;
		 centerK.y = 0;
	 }

	 Pixel<BGR> partialPix = ApplyKernel(kernel, centerK, currPoint, dims, data);
	 return partialPix.norm();





	 Pixel<BGR> partialPix;
	 Point centerK(0, 0);
	 if (!isX) {
		 partialPix = ApplyKernel(FORWARDDIFFY, centerK, currPoint, dims, data);
	 }
	 else {
		 partialPix = ApplyKernel(FORWARDDIFFX, centerK, currPoint, dims, data);
	 }
	 return partialPix.norm();
	 */
	 int step = 1;
	 if (!isX) {
		 step = dims.x;
	 }
	 int idx = currPoint.y * dims.x + currPoint.x;
	 Pixel<BGR> partialPix = (data[idx + 2 * step].scale(-1) + data[idx + 1 * step].scale(4) + data[idx].scale(-3)).scale(1 / 2.0);
	 return partialPix.norm();

 }

 //same role as centered diff but used for pixels that are on or one pixel away from the top or right borders
 //returns magnitude of partial derivative pixel (partial derivative pixel contains partial derivatives of r, g, and b)
 double ImageProcessing::backwardDiff(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims, bool isX) {
	 /*
	 std::vector<std::vector<double>> kernel = { {1/2.0, -4/2.0, 3/2.0} };
	 Point centerK(2, 0);
	 if (!isX) {
		 kernel = { {1 / 2.0},
					{-4 / 2.0},
					{3 / 2.0} };
		 centerK.x = 0;
		 centerK.y = 2;
	 }
	 ApplyKernel(kernel, centerK, currPoint, dims, data);




	 Pixel<BGR> partialPix;
	 Point centerK(2, 0);
	 if (!isX) {
		 centerK.x = 0;
		 centerK.y = 2;
		 partialPix = ApplyKernel(BACKDIFFY, centerK, currPoint, dims, data);
	 }else {
		 partialPix = ApplyKernel(BACKDIFFX, centerK, currPoint, dims, data);
	 }
	 return partialPix.norm();
	 */
	 int step = 1;
	 if (!isX) {
		 step = dims.x;
	 }
	 int idx = currPoint.y * dims.x + currPoint.x;
	 Pixel<BGR> partialPix = (data[idx].scale(3) + data[idx - 1 * step].scale(-4) + data[idx - 2 * step]).scale(1 / 2.0);
	 return  partialPix.norm();
 }



 
 // - LTIR - /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 //cant export single channel bmp from 3 chan - fix later


 void ImageProcessing::Convolution(Image<BGR>& im, std::function<Pixel<BGR>(Point, Point, const std::vector<Pixel<BGR>>&)> kernel) {
	 Point dims(im.getWidth(), im.getHeight());
	 std::vector<Pixel<BGR>> dataCopy = im.getData();
	 for (int y = 0; y < dims.y; y++) {
		 for (int x = 0; x < dims.x; x++) {
			 Point currPoint(x, y);
			 im.setPixel(x, y, kernel(currPoint, dims, dataCopy));
		 }
		 if (y == 510) {
			 std::cout << y << std::endl;
		 }
	 }
 }

 Pixel<BGR> ImageProcessing::EdgeDetectionKernel(Point currPoint, Point dims, const std::vector<Pixel<BGR>>& imData) {
	 double pX = partialX(imData, currPoint, dims);
	 double pY = partialY(imData, currPoint, dims);
	 double gradientMag = sqrt(pX * pX + pY * pY); //should multiply each partial by dx and dy but they are both one since increments are all 1 pixel
	 if (gradientMag > EDGE_DETECTION_THRESH) {
		 Pixel<BGR> whitePix({ 255, 255, 255 });
		 return whitePix;
	 }
	 Pixel<BGR> blackPix({ 0, 0, 0 });
	 return blackPix;
 }

 Pixel<BGR> ImageProcessing::LowPassFilterKernel(int x, int y, int height, int width, const std::vector<Pixel<BGR>>& imData) {
	 Pixel<BGR> total = Pixel<BGR>({ 0, 0, 0 });
	 int pixelCount = 0;
	 for (int currY = y - KERNEL_RADIUS; currY <= y + KERNEL_RADIUS; currY++) {
		 if (currY < 0 || currY > height - 1) continue;
		 for (int currX = x - KERNEL_RADIUS; currY <= y + KERNEL_RADIUS; currY++) {
			 if (currX < 0 || currX > width - 1) continue;
			 total = total + imData[currX + width * currY];
			 pixelCount++;
		 }
	 }
	 //or
	 std::vector<std::vector<int>> kernel
		 = { {1, 1, 1},
			 {1, 1, 1},
			 {1, 1, 1} };
	 //ApplyKernel(kernel, x, y, height, width, imData);

	 Pixel<BGR> avg = total.scale(1.0 / pixelCount);
	 if (avg.norm() > 57) {
		 Pixel<BGR> whitePix({ 255, 255, 255 });
		 return whitePix;
	 }
	 Pixel<BGR> blackPix({ 0, 0, 0 });
	 return blackPix;
 }
 


 /*
 //calculares partial derivative of a pixel wrt x by summing partial derivatives of r, g, and b channels wrt x
 double ImageProcessing::partialX(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims) {
	 if (currPoint.x < 2) {
		 return forwardDiff(data, currPoint, dims, 1);
	 }
	 else if (currPoint.x > dims.x - 3) {
		 return backwardDiff(data, currPoint, dims, 1);
	 }
	 return centeredDiff(data, currPoint, dims, 1);
 }

 //calculates partial derivative of a pixel wrt y by summing partial derivatives of r, g, and b channels wrt y
 double ImageProcessing::partialY(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims) {
	 if (currPoint.y < 2) {
		 return forwardDiff(data, currPoint, dims, 0);
	 }
	 else if (currPoint.y > dims.y - 3) {
		 return backwardDiff(data, currPoint, dims, 0);
	 }
	 return centeredDiff(data, currPoint, dims, 0);
 }

 //calculates centered difference derivative of r, g, and b streams, and returns the magnitude of these
 //idx to index of pixel to calcualte derivative about
 //dataIncrement is the number of vector indices between adjacent function values (image pixels) used in the numerical calculation - 1 for x and width for y, since moving in the x direction is 
 //equivalent to moving one index, and moving in the y direction is the equivalent to moving 'width' indicies in the vector
 double ImageProcessing::centeredDiff(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims, bool isX) {
	 std::vector<std::vector<double>> kernel = { {1/12.0, -8/12.0, 0, 8/12.0, -1/12.0} };
	 Point centerK(2, 0); //in x - y coordinates, now row, col
	 if (!isX) {
		 kernel = { {1/12.0},
					{-8/12.0},
					{0},
					{8/12.0}, 
					{-1/12.0} };
		 centerK.x = 0;
		 centerK.y = 2;
	 }
	 Pixel<BGR> partialPix = ApplyKernel(kernel, centerK, currPoint, dims, data);
	 return partialPix.norm();
 }

 //same role as centered diff but used for pixels that are on or one pixel away from the bottom or left borders
 double	ImageProcessing::forwardDiff(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims, bool isX) {
	 std::vector<std::vector<double>> kernel = { {-3/2.0, 4/2.0, -1/2.0} };
	 Point centerK(0, 0);
	 if (!isX) {
		 kernel = { {-3 / 2.0},
					{4 / 2.0},
					{-1 / 2.0} };
		 centerK.x = 0;
		 centerK.y = 0;
	 }
	 
	 Pixel<BGR> partialPix = ApplyKernel(kernel, centerK, currPoint, dims, data);
	 return partialPix.norm();
 }

 //same role as centered diff but used for pixels that are on or one pixel away from the top or right borders
 //returns magnitude of partial derivative pixel (partial derivative pixel contains partial derivatives of r, g, and b)
 double ImageProcessing::backwardDiff(const std::vector<Pixel<BGR>>& data, Point currPoint, Point dims, bool isX) {
	 std::vector<std::vector<double>> kernel = { {1/2.0, -4/2.0, 3/2.0} };
	 Point centerK(2, 0);
	 if (!isX) {
		 kernel = { {1 / 2.0},
					{-4 / 2.0},
					{3 / 2.0} };
		 centerK.x = 0;
		 centerK.y = 2;
	 }
	 
	 Pixel<BGR> partialPix = ApplyKernel(kernel, centerK, currPoint, dims, data);
	 return partialPix.norm();
 }
 */
 


