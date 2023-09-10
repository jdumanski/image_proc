#include "Point.hpp"

Point::Point(int x, int y) : x(x), y(y) {}

Point Point::operator+(const Point& rhs) {
	Point sum(0, 0);
	sum.x = x + rhs.x;
	sum.y = y + rhs.y;
	return sum;
}

Point Point::operator-(const Point& rhs){
	Point diff(0, 0);
	diff.x = x - rhs.x;
	diff.y = y - rhs.y;
	return diff;
}