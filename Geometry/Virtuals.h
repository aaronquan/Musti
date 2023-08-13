#pragma once

#include <memory>
#include <Eigen/Dense>

#include "Util.h"
#include "DrawShapes.h"
#include "Primatives.h"
#include "../Window/Util.h"

using namespace Eigen;


//class VirtualObject {
//public:
//	virtual void move(Vector2f movement) = 0;
//	virtual shared_ptr<DrawObject> toDrawObject(ID2D1Brush* b) = 0;
//};
//
//class VirtualShape : public VirtualObject {
//public:
//	//virtual void move(Vector2f movement) = 0;
//	//virtual shared_ptr<DrawObject> toDrawObject(ID2D1Brush* b) = 0;
//	//virtual void draw(ID2D1HwndRenderTarget* rt) = 0;
//	//virtual shared_ptr<DrawObject> toDrawObject(Array2d scale, Array2d translate) = 0;
//};

//class VirtualRectangleI;

class VirtualLine : public VirtualShape {
private:
	VirtualPoint2F start;
	VirtualPoint2F end;

public:
	VirtualLine();
	VirtualLine(Array2f p1, Array2f p2, ID2D1Brush* b = nullptr);
	VirtualLine(Array2f p1, Vector2f v, ID2D1Brush* b = nullptr);

	Array2f getStart() const;
	void setStart(Array2f st);

	Array2f getEnd() const;
	void setEnd(Array2f st);

	float side(Array2f pt) const;
	static float side(Array2f pt, Array2f p1, Array2f p2); // d == 0 on line

	bool isPointInside(Array2f p) const;
	Region getPointRegion(Array2f p) const;
	void translate(Vector2f movement);
	void translateStart(Vector2f movement);
	void translateEnd(Vector2f movement);

	void draw(ID2D1HwndRenderTarget* rt) const;
	void fill(ID2D1HwndRenderTarget* rt) const{};
};

class VirtualRectangle : public VirtualShape {
private:
	VirtualPoint2F left_top;
	VirtualPoint2F right_bottom;

	//Array2f dimensions;
	VirtualVector2F dimensions;

public:
	VirtualRectangle();
	VirtualRectangle(Vector2f dims);
	VirtualRectangle(Array2f lt, Vector2f dims, ID2D1Brush* b = nullptr);
	VirtualRectangle(VirtualPoint2F lt, Vector2f dims, ID2D1Brush* b = nullptr);
	VirtualRectangle(Array2f lt, Array2f rb, ID2D1Brush* b = nullptr); //two points, expects lt to be left_top
	//VirtualRectangle(VirtualRectangleI vr);

	bool isPointInside(Array2f p) const;
	bool isPointInside(Array2i p) const;
	Region getPointRegion(Array2f p) const;
	bool isIntersecting(VirtualRectangle v) const;



	Array2f getLeftTop() const;
	Array2f getRightBottom() const;
	void setDimensions(Vector2f dims);
	Vector2f getDimensions() const;

	void setLeftTopPosition(Array2f pos); // move 
	void translate(Vector2f movement);

	VirtualRectangle intersection(VirtualRectangle v) const;

	Array2f scaleDifference(VirtualRectangle vr) const;

	bool connect(shared_ptr<VirtualRectangle> vr, Region r=Region::RIGHT, Alignment ali=CENTRE);
	bool connect(VirtualRectangle& vr, Region region, Alignment align);

	//shared_ptr<DrawObject> toDrawObject(Array2d scale, Array2d translate);
	//shared_ptr<DrawShape> toDrawObject(ID2D1Brush* b) const;
	//void toDrawObject(shared_ptr<DrawShape>& obj, ID2D1Brush* b) const;
	void draw(ID2D1HwndRenderTarget* rt) const;
	void fill(ID2D1HwndRenderTarget* rt) const;
	string toString() const;

};

//class VirtualRectangle2 : public VirtualShape {

//};
/*
class VirtualRectangleI : public VirtualShape {
private:
	VirtualPoint2I left_top;
	VirtualPoint2I right_bottom;

	VirtualVector2I dimensions;
public:
	VirtualRectangleI();
	VirtualRectangleI(Vector2i dims);
	VirtualRectangleI(Array2i lt, Vector2i dims);
	VirtualRectangleI(VirtualPoint2I lt, Vector2i dims);
	VirtualRectangleI(Array2i lt, Array2i rb); //two points

	bool isPointInside(Array2f p) const;
	Array2f getLeftTop() const;
	Array2f getRightBottom() const;
	void setDimensions(Vector2f dims);
	Vector2f getDimensions() const;

	void move(Vector2f movement);

	void toDrawObject(shared_ptr<DrawShape>& obj, ID2D1Brush* b) const = 0;
	string toString() const = 0;
};
*/
class VirtualCircle : public VirtualShape {
private:
	VirtualPoint2F centre;
	float radius_x;
	float radius_y;
public:
	VirtualCircle();
	VirtualCircle(Array2f ce, float r, ID2D1Brush* b = nullptr);
	VirtualCircle(Array2f ce, float rx, float ry, ID2D1Brush* b = nullptr);

	Array2f getCentre() const;
	void setCentre(Array2f pt);

	void translate(Vector2f movement);
	bool isPointInside(Array2f point) const;
	Region getPointRegion(Array2f point) const;
	void draw(ID2D1HwndRenderTarget* rt) const;
	void fill(ID2D1HwndRenderTarget* rt) const;
};

class VirtualTriangle : public VirtualShape {
private:
	static ID2D1Factory* factory;
	ID2D1PathGeometry* path_geo;
	ID2D1GeometrySink* geo_sink;

	VirtualPoint2F point1;
	VirtualPoint2F point2;
	VirtualPoint2F point3;
public:
	VirtualTriangle();
	VirtualTriangle(Array2f p1, Array2f p2, Array2f p3, ID2D1Brush* b=nullptr);
	Array2f getP1() const;
	Array2f getP2() const;
	Array2f getP3() const;
	vector<Array2f> getPoints() const;

	static void createResources(ID2D1Factory* df);

	void translate(Vector2f movement);
	bool isPointInside(Array2f point) const;
	Region getPointRegion(Array2f point) const;

	void draw(ID2D1HwndRenderTarget* rt) const;
	void fill(ID2D1HwndRenderTarget* rt) const;
};

//class

//class PathGeometry : public 

/*
class ConnectedRectangles : public VirtualGroup {
private:
	vector<shared_ptr<VirtualRectangle>> rectangles;
	shared_ptr<VirtualRectangle> left_rect;
	shared_ptr<VirtualRectangle> top_rect;
	shared_ptr<VirtualRectangle> right_rect;
	shared_ptr<VirtualRectangle> bottom_rect;
public:
	ConnectedRectangles();
	bool connectRectangle(shared_ptr<VirtualRectangle> vr, Region r=Region::RIGHT, Alignment ali=CENTRE);
	bool connectRectangle(ConnectedRectangles cr, Region r = Region::RIGHT);
	shared_ptr<VirtualRectangle> getRectangle(unsigned int i) const;
	vector<shared_ptr<VirtualRectangle>> getRectangles() const;
	//shared_ptr<VirtualRectangle> 

	//void toDrawObject(shared_ptr<DrawShape>& obj, ID2D1Brush* b) const;
	virtual string toString() const;
};

*/