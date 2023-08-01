#pragma once

#include "Util.h"

string array2fToString(Array2f arr) {
	stringstream ss;
	ss << "x: " << arr(0) << ", y: " << arr(1);
	return ss.str();
}

string vector2fToString(Vector2f vec) {
	stringstream ss;
	ss << "x: " << vec(0) << ", y: " << vec(1);
	return ss.str();
}

string array2iToString(Array2i arr) {
	stringstream ss;
	ss << "x: " << arr(0) << ", y: " << arr(1);
	return ss.str();
}

string vector2iToString(Vector2i vec) {
	stringstream ss;
	ss << "x: " << vec(0) << ", y: " << vec(1);
	return ss.str();
}

wstring array2fToWString(Array2f arr) {
	wstringstream ss;
	ss << "x: " << arr(0) << ", y: " << arr(1);
	return ss.str();
}

wstring vector2fToWString(Vector2f vec) {
	wstringstream ss;
	ss << "x: " << vec(0) << ", y: " << vec(1);
	return ss.str();
}

wstring array2iToWString(Array2i arr) {
	wstringstream ss;
	ss << "x: " << arr(0) << ", y: " << arr(1);
	return ss.str();
}

Array2f randArray2f(unsigned int xRange, unsigned int yRange) {
	if(xRange == 0 || yRange == 0) return Array2f();
	return Array2f(rand() % xRange, rand() % yRange);
}

wstring vector2iToWString(Vector2i vec) {
	wstringstream ss;
	ss << "x: " << vec(0) << ", y: " << vec(1);
	return ss.str();
}

D2D1_POINT_2F array2fToPoint2F(Array2f p) {
	return Point2F(p(0), p(1));
}

Array2f point2FToArray2f(D2D1_POINT_2F p) {
	return Array2f(p.x, p.y);
}