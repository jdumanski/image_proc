#pragma once
#include <vector>
#include <stdint.h>
#include <stdexcept>

enum PixelType { //type corresponds to num of channels
	BGR = 3,
	Grayscale = 1,
	Binary = 1,
	DBGR = 4
};

template <PixelType p>
struct Pixel {
	//by convention, indices 0, 1, and 2 are BGR
	int data[p];

	Pixel()
	{
		std::fill(data, data + p, 0);
	}

	Pixel(std::initializer_list<uint8_t> _data)
	{
		int iters = 0;
		for (auto it = _data.begin(); it != _data.end(); it++) {
			if (iters >= p) {
				throw std::runtime_error("Too many arguements for pixel constructor");
			}
			data[iters] = *it;
			iters++;
		}
	}

	inline int r() const {
		return data[2];
	}
	inline int g() const {
		return data[1];
	}
	inline int b() const {
		return data[0];
	}
	
	int& operator[](int i) { //can read and write like an array element (bc returned by reference)
		if (i >= p) {
			return data[p - 1];
		}
		else if (i < 0) {
			return data[0];
		}
		return data[i];
	}

	int& operator[](int i) const { //returns constant reference to array element - allows pixel access for const ref images
		if (i >= p) {
			return const_cast<int&>(data[p - 1]);
		}
		else if (i < 0) {
			return const_cast<int&>(data[0]);
		}
		return const_cast<int&>(data[i]);
	}

	double norm() const {
		return sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
	}

	Pixel<p> scale(double val) const {
		Pixel<p> scaled({ 0, 0, 0 });
		for (int i = 0; i < p; i++) {
			scaled[i] = (int)round(data[i] * val);
		}
		return scaled;
	}

	Pixel operator+(const Pixel<p>& pix) const {
		Pixel<p> sum;
		for (int i = 0; i < p; i++) {
			sum[i] = data[i] + pix[i];
		}
		return sum;
	}

	Pixel operator-(const Pixel<p>& pix) const {
		Pixel<p> sum;
		for (int i = 0; i < p; i++) {
			sum[i] = data[i] - pix[i];
		}
		return sum;
	}

	Pixel operator*(const Pixel<p>& pix) const {
		Pixel<p> pdt;
		for (int i = 0; i < p; i++) {
			pdt[i] = data[i] * pix[i];
		}
		return pdt;
	}

	void operator+=(const Pixel<p>& pix) {
		(*this) = (*this) + pix;
	}

	double average() {
		return (data[0] + data[1] + data[2]) / 3.0;
	}



};