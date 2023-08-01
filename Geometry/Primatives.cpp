#pragma once

#include "Primatives.h"
#include "../Window/Util.h"


bool Region::isLeft() const {
	return (value == LEFT || value == LEFT_TOP || value == LEFT_BOTTOM);
}

bool Region::isTop() const {
	return (value == TOP || value == LEFT_TOP || value == RIGHT_TOP);
}

bool Region::isRight() const {
	return (value == RIGHT || value == RIGHT_TOP || value == RIGHT_BOTTOM);
}

bool Region::isBottom() const {
	return (value == BOTTOM || value == LEFT_BOTTOM || value == RIGHT_BOTTOM);
}

string Region::toString() const {
	switch (value) {
	case INSIDE: return "INSIDE";
	case LEFT: return "LEFT";
	case TOP: return "TOP";
	case RIGHT: return "RIGHT";
	case BOTTOM: return "BOTTOM";
	case LEFT_TOP: return "LEFT_TOP";
	case RIGHT_TOP: return "RIGHT_TOP";
	case RIGHT_BOTTOM: return "RIGHT_BOTTOM";
	case LEFT_BOTTOM: return "LEFT_BOTTOM";
	}
	return "INSIDE";
}

wstring Region::toWString() const {
	switch (value) {
	case INSIDE: return L"INSIDE";
	case LEFT: return L"LEFT";
	case TOP: return L"TOP";
	case RIGHT: return L"RIGHT";
	case BOTTOM: return L"BOTTOM";
	case LEFT_TOP: return L"LEFT_TOP";
	case RIGHT_TOP: return L"RIGHT_TOP";
	case RIGHT_BOTTOM: return L"RIGHT_BOTTOM";
	case LEFT_BOTTOM: return L"LEFT_BOTTOM";
	}
	return L"INSIDE";
}

string Region::regionToString(Region r) {
	switch (r.value) {
	case INSIDE: return "INSIDE";
	case LEFT: return "LEFT";
	case TOP: return "TOP";
	case RIGHT: return "RIGHT";
	case BOTTOM: return "BOTTOM";
	case LEFT_TOP: return "LEFT_TOP";
	case RIGHT_TOP: return "RIGHT_TOP";
	case RIGHT_BOTTOM: return "RIGHT_BOTTOM";
	case LEFT_BOTTOM: return "LEFT_BOTTOM";
	}
	return "INSIDE";
}

wstring Region::regionToWString(Region r) {
	switch (r.value) {
	case INSIDE: return L"INSIDE";
	case LEFT: return L"LEFT";
	case TOP: return L"TOP";
	case RIGHT: return L"RIGHT";
	case BOTTOM: return L"BOTTOM";
	case LEFT_TOP: return L"LEFT_TOP";
	case RIGHT_TOP: return L"RIGHT_TOP";
	case RIGHT_BOTTOM: return L"RIGHT_BOTTOM";
	case LEFT_BOTTOM: return L"LEFT_BOTTOM";
	}
	return L"INSIDE";
}

Region Region::stringToRegion(const string s) {
	const char* c = s.c_str();
	if (strcmp(c, "INSIDE")) return Region(INSIDE);
	if (strcmp(c, "LEFT")) return Region(LEFT);
	if (strcmp(c, "TOP")) return Region(TOP);
	if (strcmp(c, "RIGHT")) return Region(RIGHT);
	if (strcmp(c, "BOTTOM")) return Region(BOTTOM);
	if (strcmp(c, "LEFT_TOP")) return Region(LEFT_TOP);
	if (strcmp(c, "RIGHT_TOP")) return Region(RIGHT_TOP);
	if (strcmp(c, "RIGHT_BOTTOM")) return Region(RIGHT_BOTTOM);
	if (strcmp(c, "LEFT_BOTTOM")) return Region(LEFT_BOTTOM);
	return Region(INSIDE);
}



VirtualPoint2F::VirtualPoint2F() {};

VirtualPoint2F::VirtualPoint2F(float x, float y) : Array2f(x, y) {};

VirtualPoint2F::VirtualPoint2F(Array2f p) : Array2f(p){};

VirtualPoint2F VirtualPoint2F::operator+(Vector2f movement) {
	const VirtualPoint2F me = (*this);
	return VirtualPoint2F(me(0)+movement(0), me(1)+movement(1));
}

VirtualPoint2F VirtualPoint2F::operator-(Vector2f movement) {
	const VirtualPoint2F me = (*this);
	return VirtualPoint2F(me(0) - movement(0), me(1) - movement(1));
}

VirtualVector2F VirtualPoint2F::differenceTo(Array2f end) {
	const VirtualPoint2F me = (*this);
	return VirtualVector2F(end-me);
}

VirtualVector2F VirtualPoint2F::difference(VirtualPoint2F start, VirtualPoint2F end) {
	return end - start;
}

Region VirtualPoint2F::getPointRegion(Array2f p) const {
	const VirtualPoint2F me = (*this);
	if (p(0) < me(0)) {
		if (p(1) < me(1)) {
			return Region::LEFT_TOP;
		}
		else if (p(1) > me(1)) {
			return Region::LEFT_BOTTOM;
		}
		return Region::LEFT;
	}
	else if (p(0) > me(0)) {
		if (p(1) < me(1)) {
			return Region::RIGHT_TOP;
		}
		else if (p(1) > me(1)) {
			return Region::RIGHT_BOTTOM;
		}
		return Region::RIGHT;
	}
	else if (p(1) < me(1)) {
		return Region::TOP;
	}
	else if (p(1) > me(1)) {
		return Region::BOTTOM;
	}
	return Region::INSIDE;
}


VirtualPoint2I::VirtualPoint2I() {};
VirtualPoint2I::VirtualPoint2I(int x, int y) : Array2i(x, y) {};
VirtualPoint2I::VirtualPoint2I(Array2f p) : VirtualPoint2I(p(0), p(1)) {};
VirtualPoint2I::VirtualPoint2I(Array2i p) : Array2i(p) {};
VirtualPoint2I VirtualPoint2I::operator+(Vector2i movement){
	const VirtualPoint2I me = (*this);
	return VirtualPoint2I(me(0) + movement(0), me(1) + movement(1));
}
VirtualPoint2I VirtualPoint2I::operator-(Vector2i movement) {
	const VirtualPoint2I me = (*this);
	return VirtualPoint2I(me(0) - movement(0), me(1) - movement(1));
}
VirtualVector2I VirtualPoint2I::differenceTo(Array2i end){
	const VirtualPoint2I me = (*this);
	return VirtualVector2I(end - me);
}
VirtualVector2I VirtualPoint2I::difference(VirtualPoint2I start, VirtualPoint2I end){
	return end - start;
};

VirtualVector2F::VirtualVector2F() {};

VirtualVector2F::VirtualVector2F(float x, float y) : Vector2f(x, y) {};

VirtualVector2F::VirtualVector2F(Array2f p) : Vector2f(p(0), p(1)) {};

VirtualVector2F VirtualVector2F::operator/(Vector2f v) const {
	const VirtualVector2F me = (*this);
	return VirtualVector2F(me(0)/v(0), me(1)/v(1));
};

VirtualVector2F VirtualVector2F::operator/(VirtualVector2F v) const {
	const VirtualVector2F me = (*this);
	return VirtualVector2F(me(0) / v(0), me(1) / v(1));
}

VirtualVector2I::VirtualVector2I() {};

VirtualVector2I::VirtualVector2I(int x, int y) : Vector2i(x, y) {};

VirtualVector2I::VirtualVector2I(Array2i p) : Vector2i(p(0), p(1)) {};

VirtualVector2I VirtualVector2I::operator/(Vector2i v) const {
	const VirtualVector2I me = (*this);
	return VirtualVector2I(me(0) / v(0), me(1) / v(1));
};

VirtualVector2I VirtualVector2I::operator/(VirtualVector2I v) const {
	const VirtualVector2I me = (*this);
	return VirtualVector2I(me(0) / v(0), me(1) / v(1));
}



DrawShape::DrawShape() : brush(NULL) {};

DrawShape::DrawShape(ID2D1Brush* b) : brush(b) {};

void DrawShape::setBrush(ID2D1Brush* b) {
	brush = b;
}

ID2D1Brush* DrawShape::getBrush() const {
	return brush;
}

VirtualShape::VirtualShape(ID2D1Brush* b) : DrawShape(b){};

/*
shared_ptr<VirtualShape> DrawShape::toVirtualShape() const {
	return nullptr;
}*/