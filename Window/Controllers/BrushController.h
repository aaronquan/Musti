#pragma once

#include <unordered_map>
#include <string>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

/*
Controller for brushes, tag brushes with a string for easy access and multiple storage of brushes
*/

using namespace std;


class BrushController {
private:
	unordered_map<string, ID2D1Brush*> brushMap;
	ID2D1RenderTarget *rt;
public:
	BrushController(ID2D1RenderTarget *rt);
	void setRenderTarget(ID2D1RenderTarget* rt);
	HRESULT addSolidBrush(string key, D2D1_COLOR_F c);
	//HRESULT setColour(string key, const )

	ID2D1Brush* getBrush(string key);
	void release();
};
