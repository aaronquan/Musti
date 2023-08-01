#pragma once

#include "Primatives.h"
#include "Virtuals.h"

using namespace std;

//class DrawShapeVir : public DrawObject {
//private:
//	unique_ptr<VirtualShape> shape;
//	ID2D1Brush* brush;
//public:
//	DrawShapeVir();
//	DrawShapeVir(unique_ptr<VirtualShape>& sh, ID2D1Brush* b);
//	void setVirtualShape(unique_ptr<VirtualShape>& sh);
//	VirtualShape getVirtualShape() const;
//	virtual void draw(ID2D1HwndRenderTarget* rt) const;
//};


//class DrawRectangleVir : public DrawShapeVir {
//protected:
//	shared_ptr<VirtualRectangle> rectangle;
//	//D2D1_RECT_F draw_rect;
//	//ID2D1Brush* brush;
//public:
//	DrawRectangleVir();
//	DrawRectangleVir(shared_ptr<VirtualRectangle> rect, ID2D1Brush* brush);
//	void setRectangle(shared_ptr<VirtualRectangle> rect);
//	void draw(ID2D1HwndRenderTarget* rt) const;
//};