#pragma once

#include "../Util.h"
#include "FontController.h"

using namespace std;
//FontController::FontController() : FontController(NULL) {};
FontController::FontController(IDWriteFactory* f) : writeFactory(f) {};

void FontController::setWriteFactory(IDWriteFactory* f) {
	writeFactory = f;
}

IDWriteFactory* FontController::getWriteFactory() const {
	return writeFactory;
}

HRESULT FontController::addFont(string key, wstring fn, float fsize) {
	IDWriteTextFormat* tf;
	HRESULT hr = writeFactory->CreateTextFormat(fn.c_str(), NULL, 
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, fsize, L"", &tf);
	if (SUCCEEDED(hr)) {
		//pair<wstring, IDWriteTextFormat*> p(id, tf);
		//fonts.insert(p);
		fonts.insert(make_pair(key, tf));
	}
	return hr;
};

IDWriteTextFormat* FontController::getFont(string key) {
	unordered_map<string, IDWriteTextFormat*>::const_iterator f = fonts.find(key);
	if (f == fonts.end()) {
		return NULL;
	}
	return f->second;
}

HRESULT FontController::setTextAlignment(string key, DWRITE_TEXT_ALIGNMENT dta) {
	IDWriteTextFormat* font = getFont(key);
	if (font) {
		return font->SetTextAlignment(dta);
	}
	return E_ABORT;
}
HRESULT FontController::setParagraphAlignment(string key, DWRITE_PARAGRAPH_ALIGNMENT dpa) {
	IDWriteTextFormat* font = getFont(key);
	if (font) {
		return font->SetParagraphAlignment(dpa);
	}
	return E_ABORT;
}


void FontController::release() {
	for (unordered_map<string, IDWriteTextFormat*>::iterator it = fonts.begin(); it != fonts.end(); it++) {
		safeRelease(&(it->second));
	}
}