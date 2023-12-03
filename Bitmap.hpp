#pragma once
#include "Image.hpp"
#include <string.h>

template <PixelType p>
class Bitmap : public Image<p> {
	Image<p> im;
	//include array of bytes of info specific to PNG
	//put bytes int
public:
	Bitmap(std::string path){
		
		
	}
};
