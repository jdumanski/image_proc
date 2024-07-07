#include "Kernel.hpp"
#include <cmath>
#include <stdexcept>

Kernel::Kernel(std::initializer_list<double> values) {
	int sq = sqrt(values.size());
	if ( sq * sq != values.size() ) {
		throw std::runtime_error("Kernel not square");
	}
	weights = values;
}

double Kernel::getWeight(int x, int y) const {
	int dim = sqrt(weights.size());
	return weights[y * dim + x];
}

double Kernel::setWeight(int x, int y, double val) {
	int dim = sqrt(weights.size());
	weights[y * dim + x] = val;
}

int Kernel::getSize() const{
	return sqrt(weights.size());
}