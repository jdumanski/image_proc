#pragma once
#include "Image.hpp"

/*
For all image processing function, we specify the pixel type of the image being passed in
as this makes sense as not every function applies to every pixel type, as allows us to not have 
to templatize the ImageProcessing class

Also this is a static class (just a library)
*/
class ImageProcessing {
public:
	static void grayScale(Image<PixelType::BGR> & im);

private:
	ImageProcessing() {} //disable constructor to make static alsss
};