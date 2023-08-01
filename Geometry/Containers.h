#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <Eigen/Dense>

#include "DrawShapes.h"
#include "Virtuals.h"

using namespace std;
using namespace D2D1;

class Stage;

class Canvas {
protected:
	vector<shared_ptr<DrawObject>> drawObjects; //ordered rendering of objects
	vector<shared_ptr<Stage>> stages;
	unordered_map<string, shared_ptr<DrawObject>> objectReference;
public:
	Canvas();
	void setDrawObjects(vector<shared_ptr<DrawObject>> v);
	void addObjectReference(string s, shared_ptr<DrawObject> o);
	shared_ptr<DrawObject> getDrawObject(string ref);
	void addStage(Stage s);
	shared_ptr<Stage> getStage(unsigned int i);
	bool isInsideStageCoordinate(unsigned int i, Array2f p);
	bool isInsideStageBoundary(unsigned int i, Array2f p);
	shared_ptr<Array2f> stageVirtualPosition(unsigned int i, Array2f pos);
	void draw(ID2D1HwndRenderTarget* rt);
};

//Nested stages not fully supported
class Stage : public DrawObject {
private:
	vector<shared_ptr<DrawObject>> drawObjects;
	VirtualRectangle stageParentBoundary;
	VirtualRectangle stageCoordinateRange;
	Array2f scaleDifference;
	DrawRectangle backgroundRectangle;
	Matrix3x2F transformationMatrix;
public:
	Stage();
	Stage(VirtualRectangle pb, VirtualRectangle cr);
	Stage(VirtualRectangle pb, VirtualRectangle cr, ID2D1Brush* bgc);
	void addObject(shared_ptr<DrawObject> vs);
	Array2f getVirtualPosition(Array2f v) const;
	bool isInsideCoordinate(Array2f coord) const; // is inside coordinateRange
	bool isInsideBoundary(Array2f p) const;
	void draw(ID2D1HwndRenderTarget* rt) const;

	shared_ptr<VirtualObject> toVirtualObject() const{
		return nullptr;
	}
};


//may not be neccessary
//class BaseWindowApp : public Canvas {
//protected:
//	//Canvas canvas;
//public:
//	BaseWindowApp();
//
//};