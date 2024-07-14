#pragma once
#include <initializer_list>
#include <vector>

/*
* kernel class
* all kernels must be square
* just fill with zeros where desired to make "not square"
*/

class Kernel {
	std::vector<double> weights;
public:
	Kernel(std::initializer_list<double> values);
	void setWeight(int x, int y, double val);
	double getWeight(int x, int y) const;
	int getSize() const;
	/*
	
	kernel space:
	-------> +x
	|
	|
	v
	+y

	ex of indices for 3x3 kernel:
	  0  1  2    
	0 a  b  c
	1 d  e  f
	2 g  h  i

	(2, 1) is f

	*/
};
