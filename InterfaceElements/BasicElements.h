#pragma once

#include <memory>
#include <functional>
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <dwrite.h>
#pragma comment(lib, "Dwrite")
#include <Eigen/Dense>

#include "../Geometry/DrawShapes.h"
#include "../Geometry/Virtuals.h"
#include "../Geometry/DrawVirtuals.h"

#include "SubElement.h"

struct TextLayoutWithMetrics {
	IDWriteTextLayout* layout;
	DWRITE_TEXT_METRICS metrics;
};

//class TextLabel {
//protected:
//	IDWriteFactory* writeFactory;
//
//	VirtualRectangle labelRectangle;
//	IDWriteTextFormat* textFormat;
//	ID2D1Brush* brush;
//	IDWriteTextLayout* textLayout;
//public:
//	TextLabel(IDWriteFactory* wf, IDWriteTextFormat* tf, ID2D1Brush* col, IDWriteTextLayout* lay);
//	void setLabel(wstring m);
//	//void setColour()
//};

class Button {
protected:
	shared_ptr<function<void()>> func;
	//function<void()> func;
public:
	Button();
	//virtual void setPosition(Array2f pos) = 0;
	virtual void setFunction(function<void()> f);
	virtual bool isInside(Array2f p) const = 0;
	virtual void activate(Array2f p) const;
	virtual void draw(ID2D1HwndRenderTarget* rt) const = 0;
};

class DrawLabelledMessage : public DrawObject{
private:
	DrawRectangle rectangle;

	IDWriteFactory* writeFactory;

	//float halfHeight;

	/*VirtualRectangle labelRectangle;
	IDWriteTextFormat* labelFont;
	ID2D1Brush* labelColour;
	IDWriteTextLayout* labelLayout;

	VirtualRectangle messageRectangle;
	IDWriteTextFormat* messageFont;
	ID2D1Brush* messageColour;
	IDWriteTextLayout* messageLayout;*/

	TextLabel label;
	TextLabel message;

	void createTexts(Array2f pt, Vector2f dims, IDWriteFactory* f,
		IDWriteTextFormat* f1, IDWriteTextFormat* f2, ID2D1Brush* lb, ID2D1Brush* mb);
public:
	DrawLabelledMessage();
	DrawLabelledMessage(Array2f lt, Vector2f dims, ID2D1Brush* rb, IDWriteFactory* f, IDWriteTextFormat* f1, IDWriteTextFormat* f2,
		ID2D1Brush* lb, ID2D1Brush* mb);
	DrawLabelledMessage(Array2f lt, Array2f rb, ID2D1Brush* b, IDWriteFactory* f, IDWriteTextFormat* f1, IDWriteTextFormat* f2,
		ID2D1Brush* lb, ID2D1Brush* mb);
	DrawLabelledMessage(DrawRectangle dr,  IDWriteFactory* f, IDWriteTextFormat* f1, IDWriteTextFormat* f2,
		ID2D1Brush* lb, ID2D1Brush* mb);
	/*
	DrawLabelledMessage(Array2f p1, Array2f p2, IDWriteFactory* f, ID2D1Brush* rb, IDWriteTextFormat* f1, IDWriteTextFormat* f2,
		ID2D1Brush* lb, ID2D1Brush* mb);
	DrawLabelledMessage(Array2f p1, Array2f v, IDWriteFactory* f, ID2D1Brush* rb, IDWriteTextFormat* f1, IDWriteTextFormat* f2,
		ID2D1Brush* lb, ID2D1Brush* mb);
	DrawLabelledMessage(Array2f p1, Array2f v, IDWriteFactory* f, ID2D1Brush* rb, IDWriteTextFormat* f1, IDWriteTextFormat* f2,
		ID2D1Brush* tb);
	*/
	void setLabel(wstring l);
	void setMessage(wstring m);
	void setPosition(Array2f pos);
	//void setSize(Vector2f v);
	void draw(ID2D1HwndRenderTarget* rt) const;
	void release();
	~DrawLabelledMessage();
};

class RectangleButton : public DrawRectangle, public Button {
protected:
	VirtualRectangle virtualButton;
public:
	RectangleButton();
	RectangleButton(Array2f lt, Vector2f dims, ID2D1Brush* rb);
	void setPosition(Array2f pos);
	bool isInside(Array2f p) const;
	void draw(ID2D1HwndRenderTarget* rt) const;
};

//put text parts of rectangle button into text rectangle button
class TextRectangleButton : public RectangleButton {
private:

	TextLabel label;
public:
	TextRectangleButton();
	TextRectangleButton(Array2f lt, Vector2f dims, ID2D1Brush* rb, IDWriteFactory* wf,
		IDWriteTextFormat* lf, ID2D1Brush* lb);
	//TextRectangleButton(DrawRectangle rect, IDWriteFactory* wf,
	//	IDWriteTextFormat* lf, ID2D1Brush* lb);
	void setPosition(Array2f pos);
	void setLabel(wstring l);
	wstring getLabel() const;
	void draw(ID2D1HwndRenderTarget* rt) const;
	void fill(ID2D1HwndRenderTarget* rt) const;
};

class DraggableShape : public DrawObject {
private:
	shared_ptr<VirtualShape> virtualShape;
	shared_ptr<DrawShape> drawShape;
	bool isDragging;
public:
	DraggableShape();
	DraggableShape(shared_ptr<DrawShape> obj);
	DraggableShape(shared_ptr<VirtualShape> shape, ID2D1Brush* b);
	//DraggableShape(shared_ptr<VirtualShape> obj, ID2D1Brush* b);
	bool isInside(Array2f p);
	void handleMouseDown(Array2f p);
	void handleMouseUp();
	void move(Vector2f v);
	void draw(ID2D1HwndRenderTarget* rt) const;
	shared_ptr<VirtualObject> toVirtualObject() const;

	//shared_ptr<VirtualShape> getVirtualShape() const;

};

class NewDragShape : public DrawObject {
private:
	//VirtualShape* shape;
	//unique_ptr<VirtualShape> shape;
	shared_ptr<VirtualShape> shape;
	bool isDragging;
public:
	NewDragShape();
	//NewDragShape(unique_ptr<VirtualShape> sh, ID2D1Brush* b=nullptr);
	NewDragShape(shared_ptr<VirtualShape> sh, ID2D1Brush* b = nullptr);
	//NewDragShape(VirtualShape* sh, ID2D1Brush* b = nullptr);
	//VirtualShape* getShape() const;
	shared_ptr<VirtualShape> getShape() const;
	//NewDragShape(unique_ptr<DrawShapeVir> dsv);
	bool isInside(Array2f p);
	void handleMouseDown(Array2f p);
	void handleMouseUp();
	bool movement(Vector2f v);
	
	void draw(ID2D1HwndRenderTarget* rt) const;
	void fill(ID2D1HwndRenderTarget* rt) const;
};



class DropDownList : public DrawObject {
private:
	vector<unique_ptr<TextRectangleButton>> drop_buttons;
	TextRectangleButton list;
	string selected;

	bool is_open;
	VirtualTriangle list_indic;

	Array2f left_top;
	Vector2f button_dims;

	VirtualRectangle buttons_rect;
	
	IDWriteFactory* write_factory;
	IDWriteTextFormat* button_font;
	ID2D1Brush* text_brush;
	ID2D1Brush* back_brush;

	function<void(unsigned int)> func;

	void triangleOpenIndicator();
	void triangleCloseIndicator();
public:
	DropDownList();
	DropDownList(Array2f lt, Vector2f bd, ID2D1Brush* bb,
	IDWriteFactory* wf, IDWriteTextFormat* f, ID2D1Brush* tb);
	void setSelected(wstring sel);
	void setFunction(function<void(unsigned int)> f);
	unsigned int addButton(wstring text=L"", function<void()> func = []() {});
	void activate(Array2f position);
	void openList();
	void closeList();
	void clearList();
	void draw(ID2D1HwndRenderTarget* rt) const;

};

class Arrow : public DrawObject {
private:
	DrawLine dl;
	Vector2f virtualLine;
	VirtualRectangle virtualLineGeometry;
public:
	Arrow(Array2f p, Vector2f v, ID2D1Brush* ab);
	void draw(ID2D1HwndRenderTarget* rt) const;
};

class Slider : public DrawObject {
private:
	
	Array2f position;
	ID2D1Brush* activatedBrush;
	ID2D1Brush* normalBrush;
	bool activated;
};