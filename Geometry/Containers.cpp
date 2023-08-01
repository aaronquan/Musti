#pragma once

#include <d2d1helper.h>

#include "Containers.h"
#include "Util.h"
#include "../Window/Util.h"

Canvas::Canvas(){};


void Canvas::setDrawObjects(vector<shared_ptr<DrawObject>> v) {
	drawObjects = v;
}

void Canvas::addObjectReference(string s, shared_ptr<DrawObject> o) {
	objectReference.insert(make_pair(s, o));
}

shared_ptr<DrawObject> Canvas::getDrawObject(string key) {
	unordered_map<string, shared_ptr<DrawObject>>::const_iterator object = objectReference.find(key);
	if (object == objectReference.end()) {
		return nullptr;
	}
	return object->second;
}

void Canvas::addStage(Stage s) {
	stages.push_back(make_shared<Stage>(s));
}

shared_ptr<Stage> Canvas::getStage(unsigned int i) {
	if (i < stages.size()) {
		return stages[i];
	}
	return nullptr;
}

bool Canvas::isInsideStageCoordinate(unsigned int i, Array2f p) {
	shared_ptr<Stage> stage = getStage(i);
	if (stage) {
		return stage->isInsideCoordinate(p);
	}
	return false;
}

bool Canvas::isInsideStageBoundary(unsigned int i, Array2f p) {
	shared_ptr<Stage> stage = getStage(i);
	if (stage) {
		return stage->isInsideBoundary(p);
	}
	return false;
}

shared_ptr<Array2f> Canvas::stageVirtualPosition(unsigned int i, Array2f pos) {
	if(i < stages.size()) {
		return make_shared<Array2f>(stages[i]->getVirtualPosition(pos));
	}
	return nullptr;
}

void Canvas::draw(ID2D1HwndRenderTarget* rt) {
	for (shared_ptr<DrawObject> ptr_obj : drawObjects) {
		ptr_obj->draw(rt);
	}
	for (shared_ptr<Stage> stage : stages) {
		stage->draw(rt);
	}
	rt->SetTransform(D2D1::Matrix3x2F::Identity());
}

Stage::Stage(): Stage(VirtualRectangle(), VirtualRectangle(), nullptr){};

Stage::Stage(VirtualRectangle pb, VirtualRectangle cr) : 
Stage(pb, cr, nullptr)
{};

Stage::Stage(VirtualRectangle pb, VirtualRectangle cr, ID2D1Brush* bgc) :
	stageParentBoundary(pb), stageCoordinateRange(cr), 
	backgroundRectangle(pb.getLeftTop(), pb.getDimensions(), bgc) 
{
	scaleDifference = stageParentBoundary.scaleDifference(stageCoordinateRange);
	Array2f translate = stageParentBoundary.getLeftTop();
	Matrix3x2F translationMatrix = Matrix3x2F::Translation(translate(0), translate(1));
	Matrix3x2F scaleMatrix = Matrix3x2F::Scale(scaleDifference(0), scaleDifference(1));
	transformationMatrix = scaleMatrix * translationMatrix;
}

void Stage::addObject(shared_ptr<DrawObject> vs) {
	drawObjects.push_back(vs);
}

Array2f Stage::getVirtualPosition(Array2f v) const {
	Matrix3x2F inversion = Matrix3x2F(transformationMatrix);
	inversion.Invert();
	D2D1_POINT_2F point = inversion.TransformPoint(array2fToPoint2F(v));
	return point2FToArray2f(point);
}

bool Stage::isInsideCoordinate(Array2f coord) const {
	return stageCoordinateRange.isPointInside(coord);
}

bool Stage::isInsideBoundary(Array2f p) const {
	return stageParentBoundary.isPointInside(p);
}

void Stage::draw(ID2D1HwndRenderTarget* rt) const {
	backgroundRectangle.draw(rt);
	D2D1_MATRIX_3X2_F current;
	rt->GetTransform(&current);
	/*Array2f translate = stageParentBoundary.getLeftTop();
	D2D1_MATRIX_3X2_F translationMatrix = D2D1::Matrix3x2F::Translation(translate(0), translate(1));
	D2D1_MATRIX_3X2_F scaleMatrix = D2D1::Matrix3x2F::Scale(scaleDifference(0), scaleDifference(1));
	transformationMatrix = scaleMatrix * translationMatrix;*/
	rt->SetTransform(transformationMatrix * current);
	for (shared_ptr<DrawObject> ptr_obj : drawObjects) {
		ptr_obj->draw(rt);
	}
}

//BaseWindowApp::BaseWindowApp() : canvas() {};