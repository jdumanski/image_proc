#pragma once
#include <vector>
#include <stdint.h>

enum PixelType { //type corresponds to num of channels
	BGR = 3,
	Greyscale = 1,
	Binary = 1,
	DBGR = 4
};

template <PixelType p>
struct Pixel {
	uint8_t data[p];

	Pixel() {}
	Pixel(std::initializer_list<uint8_t> _data){
		int iters = 0;
		for (auto it = _data.begin(); it != _data.end(); it++) {
			if (iters >= p) {
				throw std::runtime_error("Too many arguements for pixel constructor");
			}
			data[iters] = *it;
			iters++;
		}
	}

	uint8_t& operator[](int i) { //can read and write like an array element (bc returned by reference)
		if (i >= p) {
			return data[p - 1];
		}
		else if (i < 0) {
			return data[0];
		}
		return data[i];
	}
};