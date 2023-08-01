#pragma once

#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <dwrite.h>
#pragma comment(lib, "Dwrite")
#include <Eigen/Dense>

#include <vector>
#include <memory>

#include "Primatives.h"
#include "../Window/Util.h"
//#include "Virtuals.h"

using namespace std;
using namespace Eigen;


//class DrawObject {
//public:
//	virtual void draw(ID2D1HwndRenderTarget* rt) = 0;
//};
//
//
//class DrawShape : public DrawObject {
//protected:
//	ID2D1Brush* brush;
//public:
//	//virtual void fill(ID2D1HwndRenderTarget* rt, ID2D1Brush* b) = 0;
//	void setBrush(ID2D1Brush* b);
//	virtual void fill(ID2D1HwndRenderTarget* rt) = 0;
//	//virtual void fill(ID2D1HwndRenderTarget* rt, Array2f translate, Array2f scale) = 0;
//	virtual void move(Array2f v) = 0; //move the shape by vector coordinates
//	DrawShape();
//	DrawShape(ID2D1Brush* b);
//	//virtual void draw(ID2D1HwndRenderTarget* rt) = 0;
//};

class DrawLine : public DrawShape {
private:
	D2D1_POINT_2F start;
	D2D1_POINT_2F end;
	//ID2D1Brush* brush;
	float line_width;
public:
	DrawLine();
	DrawLine(Array2f p1, Array2f p2, ID2D1Brush* b, float w);
	DrawLine(Array2f p1, Array2f p2, ID2D1Brush* b);
	//void fill(ID2D1HwndRenderTarget* rt, ID2D1Brush* b);
	void moveToStart(Array2f); //moves start to point
	void moveToEnd(Array2f); // moves end to point
	void move(Array2f v); // move line by v
	void fill(ID2D1HwndRenderTarget* rt) const;
	void draw(ID2D1HwndRenderTarget* rt) const;
	shared_ptr<VirtualShape> toVirtualShape() const;
};

class DrawEllipse : public DrawShape {
private:
	D2D1_ELLIPSE ellipse;
	//ID2D1Brush* brush;
public:
	DrawEllipse();
	DrawEllipse(Array2f p, float rx, float ry);
	DrawEllipse(Array2f p, float rx, float ry, ID2D1Brush* b);
	//void setBrush(ID2D1Brush* b);
	void move(float x, float y);
	void move(Array2f v);
	//void fill(ID2D1HwndRenderTarget* rt, ID2D1Brush* b);
	void fill(ID2D1HwndRenderTarget* rt) const;
	void draw(ID2D1HwndRenderTarget* rt) const;
	shared_ptr<VirtualShape> toVirtualShape() const;
};

class DrawRectangle : public DrawShape {
protected:
	D2D1_RECT_F rect;
	//ID2D1Brush* brush;
public:
	DrawRectangle();
	DrawRectangle(Array2f lt, Array2f rb, ID2D1Brush* b);
	DrawRectangle(Array2f lt, Vector2f dims, ID2D1Brush* b);
	//DrawRectangle(float l, float t, float r, float b, ID2D1Brush* br);
	//void setBrush(ID2D1Brush* b);
	void move(Array2f v);
	void reposition(Array2f p);
	//void fill(ID2D1HwndRenderTarget* rt, ID2D1Brush* b);
	void fill(ID2D1HwndRenderTarget* rt) const;
	void draw(ID2D1HwndRenderTarget* rt) const;
	//shared_ptr<VirtualShape> toVirtualShape() const;
};

class GeometryDrawer{
private:
	ID2D1PathGeometry* path_geo;
	ID2D1GeometrySink* geo_sink;

	bool create(ID2D1Factory* f);
public:
	GeometryDrawer();
	GeometryDrawer(ID2D1GeometrySink* sink);
	void release();
	void draw(ID2D1HwndRenderTarget* rt, vector<Array2f> points) const;
};

class SimpleGeometry {
private:
	static ID2D1Factory* factory;
	ID2D1PathGeometry* path_geo;
	ID2D1GeometrySink* geo_sink;

	vector<Array2f> points;
	ID2D1Brush* brush;
public:
	SimpleGeometry();
	SimpleGeometry(vector<Array2f> pts, ID2D1Brush* brush);

	static void createResources(ID2D1Factory* df);
	void draw(ID2D1HwndRenderTarget* rt) const;

};

class DrawGroup : public DrawObject {
protected:
	vector<DrawObject> objectGroup;
public:
	DrawGroup();
	
	void draw(ID2D1HwndRenderTarget* rt) const;
	shared_ptr<VirtualShape> toVirtualShape() const;
};