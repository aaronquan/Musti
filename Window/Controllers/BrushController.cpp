#pragma once

#include "../Util.h"
#include "BrushController.h"

BrushController::BrushController(ID2D1RenderTarget* r) : rt(r) {};

void BrushController::setRenderTarget(ID2D1RenderTarget* r) {
	rt = r;
}

HRESULT BrushController::addSolidBrush(string key, D2D1_COLOR_F c) {
	ID2D1SolidColorBrush* b;
	HRESULT hr = rt->CreateSolidColorBrush(c, &b);
	if (SUCCEEDED(hr)) {
		brushMap.insert(make_pair(key, b));
	}
	return hr;
}

ID2D1Brush* BrushController::getBrush(string key) {
	unordered_map<string, ID2D1Brush*>::const_iterator f = brushMap.find(key);
	if(f==brushMap.end()) return NULL;
	return f->second;
}

void BrushController::release() {
	//works to release br is a reference
	for (pair<string, ID2D1Brush*> br: brushMap) {
		safeRelease(&(br.second));
	}
}