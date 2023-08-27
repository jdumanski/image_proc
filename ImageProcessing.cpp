#include "ImageProcessing.hpp"

void ImageProcessing::grayScale(Image<PixelType::BGR> & im) {
	for (int y = 0; y < im.getHeight(); y++) {
		for (int x = 0; x < im.getWidth(); x++) {
			Pixel<PixelType::BGR>& pix = im.getPixel(x, y);
			uint8_t greyVal = (pix[0] + pix[1] + pix[2]) / 3.0;
			pix[0] = greyVal;
			pix[1] = greyVal;
			pix[2] = greyVal;	
		}
	}
}