#pragma once

struct Point {
	int x;
	int y;
	Point(int x, int y);
	Point operator+(const Point& rhs);
	Point operator-(const Point& rhs);
};
