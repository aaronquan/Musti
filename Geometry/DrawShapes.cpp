#pragma once

#include "Util.h"
#include "DrawShapes.h"

/*
DrawShape::DrawShape() : brush(NULL) {};

DrawShape::DrawShape(ID2D1Brush* b) : brush(b) {};

void DrawShape::setBrush(ID2D1Brush* b) {
	brush = b;
}

*/

DrawLine::DrawLine() : DrawLine(Array2f(0, 0), Array2f(0, 0), NULL, 1) {};

DrawLine::DrawLine(Array2f p1, Array2f p2, ID2D1Brush* b, float w) 
: start(D2D1::Point2F(p1(0), p1(1))), end(D2D1::Point2F(p2(0), p2(1))), line_width(w)
{
	brush = b;
};

DrawLine::DrawLine(Array2f p1, Array2f p2, ID2D1Brush* b) 
: DrawLine(p1, p2, b, 1)
{};

void DrawLine::moveToStart(Array2f p) {
	start = D2D1::Point2F(p(0), p(1));
}

void DrawLine::moveToEnd(Array2f p) {
	end = D2D1::Point2F(p(0), p(1));
}

void DrawLine::move(Array2f v) {
	start = D2D1::Point2F(start.x + v(0), start.y + v(1));
	end = D2D1::Point2F(end.x + v(0), end.y + v(1));
}

void DrawLine::fill(ID2D1HwndRenderTarget* rt) const {
	if (brush != NULL) {
		rt->DrawLine(start, end, brush, line_width);
	}
}

void DrawLine::draw(ID2D1HwndRenderTarget* rt) const {
	if (brush != NULL) {
		rt->DrawLine(start, end, brush, line_width);
	}
}

shared_ptr<VirtualShape> DrawLine::toVirtualShape() const {
	return nullptr;
}


DrawEllipse::DrawEllipse() : DrawEllipse(Array2f(0, 0), 1, 1) {};

DrawEllipse::DrawEllipse(Array2f p, float rx, float ry) : DrawEllipse(p, rx, ry, NULL) {};

DrawEllipse::DrawEllipse(Array2f p, float rx, float ry, ID2D1Brush* b) : 
	ellipse(D2D1::Ellipse(D2D1::Point2F(p(0), p(1)), rx, ry)) {
	brush = b;
};

void DrawEllipse::move(Array2f v) {
	ellipse = D2D1::Ellipse(D2D1::Point2F(ellipse.point.x+v(0), ellipse.point.y+v(1)),ellipse.radiusX, ellipse.radiusY);
}

void DrawEllipse::fill(ID2D1HwndRenderTarget* rt) const {
	if (brush != NULL) {
		rt->FillEllipse(ellipse, brush);
	}
}

void DrawEllipse::draw(ID2D1HwndRenderTarget* rt) const {
	if (brush != NULL) {
		rt->DrawEllipse(ellipse, brush);
	}
}

shared_ptr<VirtualShape> DrawEllipse::toVirtualShape() const {
	return nullptr;
}

DrawRectangle::DrawRectangle() : DrawRectangle(Array2f(0, 0), Array2f(0, 0), NULL) {};

DrawRectangle::DrawRectangle(Array2f lt, Array2f rb, ID2D1Brush* b) 
: rect(D2D1::RectF(lt(0), lt(1), rb(0), rb(1))) {
	brush = b;
};

DrawRectangle::DrawRectangle(Array2f lt, Vector2f dims, ID2D1Brush* b)
: rect(D2D1::RectF(lt(0), lt(1), lt(0)+dims(0), lt(1) + dims(1))) {
	brush = b;
};


void DrawRectangle::move(Array2f v) {
	rect = D2D1::RectF(rect.left + v(0), rect.top + v(1), rect.right + v(0), rect.bottom + v(1));
}

void DrawRectangle::reposition(Array2f p) {
	float width = rect.right - rect.left;
	float height = rect.bottom - rect.top;
	rect = D2D1::RectF(p(0), p(1), p(0)+width, p(1)+height);
}

void DrawRectangle::fill(ID2D1HwndRenderTarget* rt) const {
	if (brush != NULL) {
		rt->FillRectangle(rect, brush);
	}
}

void DrawRectangle::draw(ID2D1HwndRenderTarget* rt) const {
	if (brush != NULL) {
		rt->DrawRectangle(rect, brush);
	}
}

/*
shared_ptr<VirtualShape> DrawRectangle::toVirtualShape() const {
	VirtualRectangle vr(Array2f(rect.left, rect.top), Array2f(rect.right, rect.bottom));
	return make_shared<VirtualRectangle>(vr);
}*/

GeometryDrawer::GeometryDrawer() : GeometryDrawer(nullptr) {};

GeometryDrawer::GeometryDrawer(ID2D1GeometrySink* sink) : geo_sink(sink) {};

void GeometryDrawer::release() {
	safeRelease(&geo_sink);
}

bool GeometryDrawer::create(ID2D1Factory* f) {
	return f->CreatePathGeometry(&path_geo);
}

//ID2D1PathGeometry* SimpleGeometry::path_geo = nullptr;
//ID2D1GeometrySink* SimpleGeometry::geo_sink = nullptr;
ID2D1Factory* SimpleGeometry::factory = nullptr;

SimpleGeometry::SimpleGeometry() : SimpleGeometry(vector<Array2f>(), nullptr) {};

SimpleGeometry::SimpleGeometry(vector<Array2f> pts, ID2D1Brush* b) : points(pts), brush(b){
	if(factory){

		factory->CreatePathGeometry(&path_geo);
		HRESULT hr = path_geo->Open(&geo_sink);
		if (points.size()) {
			geo_sink->BeginFigure(D2D1::Point2F(points[0](0), points[0](1)), D2D1_FIGURE_BEGIN_FILLED);
			for (unsigned int i = 1; i < points.size(); ++i) {
				geo_sink->AddLine(D2D1::Point2F(points[i](0), points[i](1)));
			}
			geo_sink->EndFigure(D2D1_FIGURE_END_CLOSED);
			geo_sink->Close();
		}
	}
};

void SimpleGeometry::createResources(ID2D1Factory* df) {
	factory = df;
	//return df->CreatePathGeometry(&path_geo);
}

void SimpleGeometry::draw(ID2D1HwndRenderTarget* rt) const {
	if(brush){
		rt->DrawGeometry(path_geo, brush, 1);
	}
}