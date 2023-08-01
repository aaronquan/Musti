#pragma once

#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <memory>
#include <string>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;


/*
 r1 | r2
----|----
 r3 | r4

   
  LT| T | RT
 -----------
  L | N | R
 ----------
  LB| B | RB

*/

class Region {
	enum Value;
private:
	Value value;
public:
	enum Value : uint8_t { INSIDE, LEFT, TOP, RIGHT, BOTTOM, 
	LEFT_TOP, RIGHT_TOP, RIGHT_BOTTOM, LEFT_BOTTOM };

	Region() = default;
	Region(Value v) : value(v) { }

	#if Enable switch(value) use case:
		// Allow switch and comparisons.
		constexpr operator Value() const { return value; }

		// Prevent usage: if(fruit)
		explicit operator bool() const = delete;
	#else
		constexpr bool operator==(Region r) const { return value == r.value; }
		constexpr bool operator!=(Region r) const { return value != r.value; }
	#endif

	bool isLeft() const;
	bool isTop() const;
	bool isRight() const;
	bool isBottom() const;

	string toString() const;
	wstring toWString() const;
	static string regionToString(Region r);
	static wstring regionToWString(Region r);
	static Region stringToRegion(string s);
};

enum Alignment {LEFT, CENTRE, RIGHT};

class VirtualVector2F;
class VirtualVector2I;

class VirtualPoint2F : public Array2f {
public:
	VirtualPoint2F();
	VirtualPoint2F(float x, float y);
	VirtualPoint2F(Array2f p);
	VirtualPoint2F(Array2i p);
	VirtualPoint2F operator+(Vector2f movement);
	VirtualPoint2F operator-(Vector2f movement);


	//VirtualPoint2F operator-(Virtual)
	VirtualVector2F differenceTo(Array2f end); //vector from this to end
	static VirtualVector2F difference(VirtualPoint2F start, VirtualPoint2F end);

	Region getPointRegion(Array2f p) const;
};

class VirtualPoint2I : public Array2i {
public:
	VirtualPoint2I();
	VirtualPoint2I(int x, int y);
	VirtualPoint2I(Array2f p);
	VirtualPoint2I(Array2i p);
	VirtualPoint2I operator+(Vector2i movement);
	VirtualPoint2I operator-(Vector2i movement);
	VirtualVector2I differenceTo(Array2i end); //vector from this to end
	//VirtualVector2F operator
	static VirtualVector2I difference(VirtualPoint2I start, VirtualPoint2I end);

};

class VirtualVector2F : public Vector2f {
public:
	VirtualVector2F();
	VirtualVector2F(float x, float y);
	VirtualVector2F(Array2f p);
	VirtualVector2F operator/(Vector2f v) const;
	VirtualVector2F operator/(VirtualVector2F v) const;
};

class VirtualVector2I : public Vector2i {
public:
	VirtualVector2I();
	VirtualVector2I(int x, int y);
	VirtualVector2I(Array2i p);
	VirtualVector2I operator/(Vector2i v) const;
	VirtualVector2I operator/(VirtualVector2I v) const;
};

class VirtualObject;
class VirtualShape;

class DrawObject {
public:
	virtual void draw(ID2D1HwndRenderTarget* rt) const = 0;
	//virtual shared_ptr<VirtualObject> toVirtualObject() const;
};

class DrawShape : public DrawObject {
protected:
	ID2D1Brush* brush;
public:
	//virtual void fill(ID2D1HwndRenderTarget* rt, ID2D1Brush* b) = 0;
	void setBrush(ID2D1Brush* b);
	ID2D1Brush* getBrush() const;
	//virtual void fill(ID2D1HwndRenderTarget* rt) = 0;
	//virtual void fill(ID2D1HwndRenderTarget* rt, Array2f translate, Array2f scale) = 0;
	//virtual void move(Array2f v) = 0; //move the shape by vector coordinates, maybe delete and put in virtuals
	DrawShape();
	DrawShape(ID2D1Brush* b);
	//virtual void draw(ID2D1HwndRenderTarget* rt) const = 0;
	virtual void fill(ID2D1HwndRenderTarget* rt) const = 0;
	//virtual shared_ptr<VirtualShape> toVirtualShape() const;
};

class VirtualObject {
public:
	virtual void translate(Vector2f movement) = 0;
	virtual bool isPointInside(Array2f point) const = 0;
	virtual Region getPointRegion(Array2f point) const = 0;
	//virtual shared_ptr<DrawObject> toDrawObject(ID2D1Brush* b) = 0;
};

class VirtualShape : public VirtualObject, public DrawShape {
protected:
public:
	VirtualShape(ID2D1Brush* b=nullptr);
	//virtual void move(Vector2f movement) = 0;
	//virtual shared_ptr<DrawObject> toDrawObject(ID2D1Brush* b) = 0;
	//virtual shared_ptr<DrawObject> toDrawObject(Array2d scale, Array2d translate) = 0;
	//virtual void toDrawObject(shared_ptr<DrawShape>& obj, ID2D1Brush* b) const = 0;
	//virtual shared_ptr<DrawShape> toDrawObject(ID2D1Brush* b) const = 0;
	//virtual void draw(ID2D1HwndRenderTarget* rt, ID2D1Brush* b) const = 0;
	//virtual string toString() const = 0;
};

class VirtualGroup : public VirtualObject {
public:
	//virtual vector<
};