#pragma once

#include "Virtuals.h"

VirtualLine::VirtualLine() : VirtualLine(Array2f(), Array2f()){};

VirtualLine::VirtualLine(Array2f p1, Array2f p2) : start(p1), end(p2){};

bool VirtualLine::isPointInside(Array2f p) const {
	return false;
}

float VirtualLine::side(Array2f pt) const {
	return (pt(0) - start(0)) * (end(1) - start(1)) - (pt(1) - start(1)) * (end(0) - start(0));
}

float VirtualLine::side(Array2f pt, Array2f p1, Array2f p2) {
	return (pt(0) - p1(0)) * (p2(1) - p1(1)) - (pt(1) - p1(1)) * (p2(0) - p1(0));
}

Region VirtualLine::getPointRegion(Array2f p) const {
	return Region::LEFT;
}
void VirtualLine::translate(Vector2f movement) {
	start = start + movement;
	end = end + movement;
}

void VirtualLine::draw(ID2D1HwndRenderTarget* rt) const {

}

VirtualRectangle::VirtualRectangle(){};

VirtualRectangle::VirtualRectangle(Vector2f dims) : VirtualRectangle(Array2f(), dims){};

VirtualRectangle::VirtualRectangle(Array2f lt, Vector2f dims) :
	left_top(lt), right_bottom(VirtualPoint2F(lt) + dims), dimensions(dims) {};

VirtualRectangle::VirtualRectangle(VirtualPoint2F lt, Vector2f dims) :
	left_top(lt), right_bottom(lt + dims), dimensions(dims) {};

VirtualRectangle::VirtualRectangle(Array2f lt, Array2f rb) : 
	left_top(lt), right_bottom(rb), dimensions(rb - lt) {
	
};

//VirtualRectangle::VirtualRectangle(VirtualRectangleI vr) : 
//VirtualRectangle(VirtualPoint2F(vr.getLeftTop()), VirtualVector2F(vr.getDimensions())) {};

bool VirtualRectangle::isPointInside(Array2f p) const {
	return (p(0) >= left_top(0) && p(0) <= right_bottom(0) && p(1) >= left_top(1) && p(1) <= right_bottom(1));
}

bool VirtualRectangle::isPointInside(Array2i p) const {
	return (p(0) >= left_top(0) && p(0) <= right_bottom(0) && p(1) >= left_top(1) && p(1) <= right_bottom(1));
}

Region VirtualRectangle::getPointRegion(Array2f p) const {
	if (p(0) < left_top(0)) {
		if (p(1) < left_top(1)) {
			return Region::LEFT_TOP;
		}
		else if (p(1) > right_bottom(1)) {
			return Region::LEFT_BOTTOM;
		}
		return Region::LEFT;
	}
	else if (p(0) > right_bottom(0)) {
		if (p(1) < left_top(1)) {
			return Region::RIGHT_TOP;
		}
		else if (p(1) > right_bottom(1)) {
			return Region::RIGHT_BOTTOM;
		}
		return Region::RIGHT;
	}
	if (p(1) < left_top(1)) {
		return Region::TOP;
	}
	else if (p(1) > right_bottom(1)) {
		return Region::BOTTOM;
	}
	return Region::INSIDE;
}

bool VirtualRectangle::isIntersecting(VirtualRectangle v) const {
	VirtualPoint2F lt = v.getLeftTop(); 
	VirtualPoint2F rb = v.getRightBottom();
	if (isPointInside(lt) || isPointInside(rb) || v.isPointInside(left_top) || v.isPointInside(right_bottom)) {
		return true;
	}
	if ((lt(0) <= left_top(0) && lt(1) <= right_bottom(1)) || (lt(0) <= right_bottom(0) && lt(1) <= left_top(1))){
		if(rb(1) >= left_top(1) && rb(0) >= right_bottom(0)) {
			return true;
		}
	}
	//to finish and test
	//if (lt(0) <= right_bottom(0) && ) {
		
	//}
	return false;
}

VirtualRectangle VirtualRectangle::intersection(VirtualRectangle v) const {
	return VirtualRectangle();
}

Array2f VirtualRectangle::getLeftTop() const {
	return left_top;
}

Array2f VirtualRectangle::getRightBottom() const {
	return right_bottom;
}

void VirtualRectangle::setDimensions(Vector2f dims) {
	dimensions = VirtualVector2F(dims);
	right_bottom = left_top + dimensions;
}

Vector2f VirtualRectangle::getDimensions() const {
	return dimensions;
}

void VirtualRectangle::setLeftTopPosition(Array2f pos) {
	left_top = pos;
	right_bottom = left_top + dimensions;
}

Array2f VirtualRectangle::scaleDifference(VirtualRectangle vr) const {
	return dimensions/vr.dimensions;
}	

void VirtualRectangle::translate(Vector2f movement) {
	left_top = left_top + movement;
	right_bottom = right_bottom + movement;

}

bool VirtualRectangle::connect(shared_ptr<VirtualRectangle> vr, Region region, Alignment align) {
	return connect(*vr, region, align);
	//Array2f new_left_top = left_top;
	//if (region == Region::RIGHT) {
	//	//outputDebugLine(to_string(align));
	//	new_left_top(0) += dimensions(0); //align left only
	//	if (align == RIGHT) { 
	//		Vector2f vr_dims = vr->getDimensions();
	//		new_left_top(1) += dimensions(1) - vr_dims(1);
	//	}
	//	if (align == CENTRE) {
	//		Vector2f vr_dims = vr->getDimensions();
	//		new_left_top(1) += (dimensions(1)/2) - (vr_dims(1)/2);
	//	}
	//	vr->setLeftTopPosition(new_left_top);
	//	return true;
	//}
	//if (region == Region::BOTTOM) {
	//	Vector2f vr_dims = vr->getDimensions();
	//	new_left_top(0) += dimensions(0) - vr_dims(0);
	//	new_left_top(1) += dimensions(1);
	//	vr->setLeftTopPosition(new_left_top);
	//	return true;
	//}
	//if (region == Region::LEFT) {
	//	Vector2f vr_dims = vr->getDimensions();
	//	new_left_top(0) -= vr_dims(0);
	//	new_left_top(1) += dimensions(1) - vr_dims(1);
	//	vr->setLeftTopPosition(new_left_top);
	//	return true;
	//}
	//if (region == Region::TOP) {
	//	Vector2f vr_dims = vr->getDimensions();
	//	new_left_top(1) -= vr_dims(1);
	//	vr->setLeftTopPosition(new_left_top);
	//	return true;
	//}
	//return false;
}

bool VirtualRectangle::connect(VirtualRectangle& vr, Region region, Alignment align) {
	Array2f new_left_top = left_top;
	if (region == Region::RIGHT) {
		//outputDebugLine(to_string(align));
		new_left_top(0) += dimensions(0); //align left only
		if (align == RIGHT) {
			Vector2f vr_dims = vr.getDimensions();
			new_left_top(1) += dimensions(1) - vr_dims(1);
		}
		if (align == CENTRE) {
			Vector2f vr_dims = vr.getDimensions();
			new_left_top(1) += (dimensions(1) / 2) - (vr_dims(1) / 2);
		}
		vr.setLeftTopPosition(new_left_top);
		return true;
	}
	if (region == Region::BOTTOM) {
		Vector2f vr_dims = vr.getDimensions();
		new_left_top(1) += dimensions(1);
		if(align == LEFT){
			new_left_top(0) += dimensions(0) - vr_dims(0);
		}
		if(align == CENTRE){
			new_left_top(0) += (dimensions(0) - vr_dims(0))/2;
		}
		vr.setLeftTopPosition(new_left_top);
		return true;
	}
	if (region == Region::LEFT) {
		Vector2f vr_dims = vr.getDimensions();
		new_left_top(0) -= vr_dims(0);
		if(align == LEFT){
			new_left_top(1) += dimensions(1) - vr_dims(1);
		}
		if (align == CENTRE) {
			new_left_top(1) += (dimensions(1) - vr_dims(1))/2;
		}
		vr.setLeftTopPosition(new_left_top);
		return true;
	}
	if (region == Region::TOP) {
		Vector2f vr_dims = vr.getDimensions();
		new_left_top(1) -= vr_dims(1);
		if (align == RIGHT) {
			new_left_top(0) += dimensions(0) - vr_dims(0);
		}
		if (align == CENTRE) {
			new_left_top(0) += (dimensions(0) - vr_dims(0))/2;
		}
		vr.setLeftTopPosition(new_left_top);
		return true;
	}
	return false;
}

/*
shared_ptr<DrawShape> VirtualRectangle::toDrawObject(ID2D1Brush* b) const {
	return make_shared<DrawRectangle>(DrawRectangle(left_top, dimensions, b));
}

void VirtualRectangle::toDrawObject(shared_ptr<DrawShape>& obj, ID2D1Brush* b) const {
	//if (obj) {
		//outputDebugLine(array2fToString(left_top));
	obj = make_shared<DrawRectangle>(DrawRectangle(left_top, dimensions, b));
	//}
}
*/

void VirtualRectangle::draw(ID2D1HwndRenderTarget* rt) const {
	//outputDebugLine("TES");
	if (brush) {
		//outputDebugLine("FILL");
		//Array2f left_top = rectangle->getLeftTop();
		//Array2f right_bottom = rectangle->getRightBottom();
		D2D1_RECT_F rect = { left_top(0), left_top(1), right_bottom(0), right_bottom(1) };
		rt->DrawRectangle(rect, brush);
	}
}

void VirtualRectangle::fill(ID2D1HwndRenderTarget* rt) const {
	if (brush) {

		D2D1_RECT_F rect = { left_top(0), left_top(1), right_bottom(0), right_bottom(1) };
		rt->FillRectangle(rect, brush);
	}
}

string VirtualRectangle::toString() const {
	stringstream ss;
	ss << "Left Top - " << array2fToString(left_top) << 
	" | Right Bottom - " << array2fToString(right_bottom) << 
	" | Dimensions - " << vector2fToString(dimensions);
	return ss.str();
}

VirtualCircle::VirtualCircle() : VirtualCircle(Array2f(), 0, nullptr){};

VirtualCircle::VirtualCircle(Array2f ce, float r, ID2D1Brush* b) : VirtualCircle(ce, r, r, b){};

VirtualCircle::VirtualCircle(Array2f ce, float rx, float ry, ID2D1Brush* b) 
: centre(ce), radius_x(rx), radius_y(ry), VirtualShape(b){};

void VirtualCircle::translate(Vector2f movement) {
	centre = centre + movement;
}

bool VirtualCircle::isPointInside(Array2f point) const {
	float dx = point(0) - centre(0); float dy = point(1) - centre(1);
	float xcal = (dx * dx) / (radius_x * radius_x);
	float ycal = (dy * dy) / (radius_y * radius_y);
	return xcal + ycal <= 1;
}

Region VirtualCircle::getPointRegion(Array2f point) const {
	return Region::LEFT;
}

void VirtualCircle::draw(ID2D1HwndRenderTarget* rt) const {
	if (brush) {
		D2D1_ELLIPSE eli = D2D1::Ellipse(D2D1::Point2F(centre(0), centre(1)), radius_x, radius_y);
		rt->DrawEllipse(eli, brush);
	}
}

void VirtualCircle::fill(ID2D1HwndRenderTarget* rt) const {
	if (brush) {
		D2D1_ELLIPSE eli = D2D1::Ellipse(D2D1::Point2F(centre(0), centre(1)), radius_x, radius_y);
		rt->FillEllipse(eli, brush);
	}
}

ID2D1Factory* VirtualTriangle::factory = nullptr;

VirtualTriangle::VirtualTriangle(): VirtualTriangle(Array2f(), Array2f(), Array2f()){};

VirtualTriangle::VirtualTriangle(Array2f p1, Array2f p2, Array2f p3, ID2D1Brush* b) : 
point1(p1), point2(p2), point3(p3), geo_sink(nullptr), path_geo(nullptr),
VirtualShape(b){
	if (factory) {
		factory->CreatePathGeometry(&path_geo);
		HRESULT hr = path_geo->Open(&geo_sink);
		geo_sink->BeginFigure(D2D1::Point2F(p1(0), p1(1)), D2D1_FIGURE_BEGIN_FILLED);
		geo_sink->AddLine(D2D1::Point2F(p2(0), p2(1)));
		geo_sink->AddLine(D2D1::Point2F(p3(0), p3(1)));
		geo_sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		geo_sink->Close();
	}
};

void VirtualTriangle::createResources(ID2D1Factory* df) {
	factory = df;
}

Array2f VirtualTriangle::getP1() const {
	return point1;
}
Array2f VirtualTriangle::getP2() const {
	return point2;
}
Array2f VirtualTriangle::getP3() const {
	return point3;
}

Region VirtualTriangle::getPointRegion(Array2f point) const {
	return Region::LEFT;
}

bool VirtualTriangle::isPointInside(Array2f point) const {
	return false;
}

void VirtualTriangle::translate(Vector2f movement) {
	point1 = point1 + movement;
	point2 = point2 + movement;
	point3 = point3 + movement;
}

void VirtualTriangle::draw(ID2D1HwndRenderTarget* rt) const {
	if (brush) {
		rt->DrawGeometry(path_geo, brush, 1);
	}
}

void VirtualTriangle::fill(ID2D1HwndRenderTarget* rt) const {
	if (brush) {
		rt->FillGeometry(path_geo, brush);
	}
}
//VirtualRectangleI::VirtualRectangleI() {};
//
//VirtualRectangle::VirtualRectangleI(Array2i lt, Vector2i dims) :
//	left_top(lt), right_bottom(VirtualPoint2F(lt) + dims), dimensions(dims) {};
//
//VirtualRectangle::VirtualRectangleI(VirtualPoint2I lt, Vector2i dims) :
//	left_top(lt), right_bottom(lt + dims), dimensions(dims) {};
//
//VirtualRectangle::VirtualRectangleI(Array2i lt, Array2i rb) :
//	left_top(lt), right_bottom(rb), dimensions(rb - lt) {};


//ConnectedRectangles::ConnectedRectangles() 
//: left_rect(nullptr), top_rect(nullptr), right_rect(nullptr), bottom_rect(nullptr){};
//
//bool ConnectedRectangles::connectRectangle(shared_ptr<VirtualRectangle> vr, Region region, Alignment ali) {
//	if (region == Region::RIGHT) {
//		Array2f lt = left_rect->getLeftTop();
//		lt(0) += left_rect->getDimensions()(0);
//		//vr->setLeftTopPosition(region.)
//	}
//	else if (region == Region::LEFT) {
//
//	}
//	else if (region == Region::TOP) {
//
//	}
//	return true;
//}