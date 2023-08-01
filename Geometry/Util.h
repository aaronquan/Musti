#pragma once

#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <string>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;
using namespace D2D1;

string array2fToString(Array2f arr);
string vector2fToString(Vector2f vec);

string array2iToString(Array2i arr);
string vector2iToString(Vector2i vec);

wstring array2fToWString(Array2f arr);
wstring vector2fToWString(Vector2f vec);

wstring array2iToWString(Array2i arr);
wstring vector2iToWString(Vector2i vec);

Array2f randArray2f(unsigned int xRange, unsigned int yRange);

D2D1_POINT_2F array2fToPoint2F(Array2f p);
Array2f point2FToArray2f(D2D1_POINT_2F p);
