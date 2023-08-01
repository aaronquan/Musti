#pragma once
#include "Util.h"

int add(int a, int b) {
	return a + b;
}

void outputDebugLine(Array2f arr) {
	outputDebugLine(array2fToString(arr));
}

void outputDebugLine(Vector2f arr) {
	outputDebugLine(vector2fToString(arr));
}

void outputDebugLine(Array2i arr) {
	outputDebugLine(array2iToString(arr));
}

void outputDebugLine(Vector2i arr) {
	outputDebugLine(vector2iToString(arr));
}

void outputDebugLine(char c) {
	string s(1, c);
	outputDebugLine(s);
}

void outputDebugLine(string s) {
	OutputDebugString((s + '\n').c_str());
}

void outputDebugLine(wstring s) {
	OutputDebugStringW((s + L'\n').c_str());
}

void outputDebugLine(vector<unsigned int> v) {
	stringstream ss;
	for (unsigned int t : v) {
		ss << to_string(t) << ' ';
	}
	outputDebugLine(ss.str());
}