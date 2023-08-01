#pragma once

#include "SubElement.h"

TextLabel::TextLabel() : TextLabel(Array2f(0, 0), Vector2f(0, 0), nullptr, nullptr, nullptr){};

TextLabel::TextLabel(Array2f lt, Vector2f dims, IDWriteFactory* wf, IDWriteTextFormat* tf, 
ID2D1Brush* br)
: rectangle(lt, dims), writeFactory(wf), format(tf), brush(br), layout(nullptr) {}

TextLabel::~TextLabel() {
	safeRelease(&layout);
}

void TextLabel::setLabel(wstring l) {
	if (writeFactory && format) {
		safeRelease(&layout);
		const Array2f labelDims = rectangle.getDimensions();
		HRESULT hr = writeFactory->CreateTextLayout(l.c_str(), l.size(), format, labelDims(0), labelDims(1), &layout);
		if (SUCCEEDED(hr)) {

		}
	}
	label = l;
}

wstring TextLabel::getLabel() const {
	return label;
}

Array2f TextLabel::getLeftTop() const {
	return rectangle.getLeftTop();
};

void TextLabel::setLeftTopPosition(Array2f p) {
	rectangle.setLeftTopPosition(p);
}

Vector2f TextLabel::getDimensions() {
	return rectangle.getDimensions();
}

void TextLabel::setBrush(ID2D1Brush* br) {
	brush = br;
}

void TextLabel::draw(ID2D1HwndRenderTarget* rt) const {
	if(layout && brush){
		const Array2f leftTop = rectangle.getLeftTop();
		rt->DrawTextLayout(D2D1::Point2F(leftTop(0), leftTop(1)), layout, brush);
	}
}