#pragma once

#include <string>
#include <windows.h>
#include <dwrite.h>
#pragma comment(lib, "Dwrite")
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "../Geometry/Virtuals.h"
#include "../Window/Util.h"

class TextLabel {
protected:
	wstring label;

	IDWriteFactory* writeFactory;

	VirtualRectangle rectangle;
	IDWriteTextFormat* format;
	ID2D1Brush* brush;
	IDWriteTextLayout* layout;
public:
	TextLabel();
	TextLabel(Array2f lt, Vector2f dims, IDWriteFactory* wf, IDWriteTextFormat* tf, ID2D1Brush* col);
	~TextLabel();
	void setLabel(wstring l);
	wstring getLabel() const;
	Array2f getLeftTop() const;
	void setLeftTopPosition(Array2f p);
	Vector2f getDimensions();
	void setBrush(ID2D1Brush* br);
	void draw(ID2D1HwndRenderTarget* rt) const;
};

class TextLabelFactory {
private:
	//IDWriteFactory* writeFactory;
public:
	static TextLabel createLabel(IDWriteFactory* wf);
};