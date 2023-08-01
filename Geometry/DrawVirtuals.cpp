#pragma once

#include "DrawVirtuals.h"

//DrawShapeVir::DrawShapeVir() : DrawShapeVir(nullptr, nullptr) {};
//
//DrawShapeVir::DrawShapeVir(unique_ptr<VirtualShape>& sh, ID2D1Brush* b)
//	: shape(move(sh)), brush(b) {};
//
//void DrawShapeVir::setVirtualShape(unique_ptr<VirtualShape>& sh) {
//	shape = move(sh);
//}
//
//VirtualShape DrawShapeVir::getVirtualShape() const {
//	return *shape;
//}
//
//void DrawShapeVir::draw(ID2D1HwndRenderTarget* rt) const {
//	if (shape){
//		shape->draw(rt, brush);
//	}
//}

//DrawRectangleVir::DrawRectangleVir(shared_ptr<VirtualRectangle> rect, ID2D1Brush* b)
//	: rectangle(rect), brush(b) {
//};
//
//void DrawRectangleVir::setRectangle(shared_ptr<VirtualRectangle> rect) {
//	rectangle = rect;
//}
//
//void DrawRectangleVir::draw(ID2D1HwndRenderTarget* rt) const {
//	outputDebugLine("TES");
//	if (rectangle && brush) {
//		outputDebugLine("FILL");
//		Array2f left_top = rectangle->getLeftTop();
//		Array2f right_bottom = rectangle->getRightBottom();
//		D2D1_RECT_F rect = { left_top(0), left_top(1), right_bottom(0), right_bottom(1) };
//		rt->FillRectangle(rect, brush);
//	}
//}