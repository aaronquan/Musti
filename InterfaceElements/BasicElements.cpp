#pragma once

#include "BasicElements.h"
#include "../Window/Util.h"

Button::Button() : func(nullptr){};

void Button::setFunction(function<void()> f) {
	func = make_shared<function<void()>>(f);
	//func = f;
}

void Button::activate(Array2f p) const {
	if (func && isInside(p)) {
		(*func)();
		//func();
	}
}


//TextLabel::TextLabel(IDWriteFactory* wf, IDWriteTextFormat* tf,
//ID2D1Brush* b, IDWriteTextLayout* tl)
//	: writeFactory(wf), textFormat(tf), brush(b), textLayout(tl) {
//
//}
//
//void TextLabel::setLabel(wstring m) {
//	if (writeFactory && textFormat) {
//		safeRelease(&textLayout);
//		const Array2f labelDims = labelRectangle.getDimensions();
//		HRESULT hr = writeFactory->CreateTextLayout(l.c_str(), l.size(), labelFont, labelDims(0), labelDims(1), &labelLayout);
//		if (SUCCEEDED(hr)) {
//
//		}
//	}
//}

DrawLabelledMessage::DrawLabelledMessage() 
: DrawLabelledMessage(Array2f(0, 0), Array2f(0, 0), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) {};

DrawLabelledMessage::DrawLabelledMessage(Array2f lt, Vector2f dims, ID2D1Brush* rb, IDWriteFactory* f,
	IDWriteTextFormat* f1, IDWriteTextFormat* f2, ID2D1Brush* lb, ID2D1Brush* mb)
	: rectangle(lt, dims, rb), writeFactory(f)
	{
	createTexts(lt, dims, f, f1, f2, lb, mb);
}

DrawLabelledMessage::DrawLabelledMessage(Array2f lt, Array2f rb, ID2D1Brush* b, IDWriteFactory* f,
IDWriteTextFormat* f1, IDWriteTextFormat* f2, ID2D1Brush* lb, ID2D1Brush* mb) : rectangle(lt, rb, b), writeFactory(f)
{
	Vector2f dims = rb - lt;
	const Vector2f virtualDims(dims(0), dims(1)/2);
	createTexts(lt, dims, f, f1, f2, lb, mb);
};

DrawLabelledMessage::DrawLabelledMessage(DrawRectangle dr, IDWriteFactory* f, IDWriteTextFormat* f1, IDWriteTextFormat* f2,
	ID2D1Brush* lb, ID2D1Brush* mb) : rectangle(dr), writeFactory(f)
	//,labelFont(f1), labelColour(lb), labelLayout(nullptr), messageFont(f2), messageColour(mb), messageLayout(nullptr) 
	{
	//shared_ptr<VirtualRectangle> r = dynamic_pointer_cast<VirtualRectangle>(dr.toVirtualShape());
	//Vector2f dims = r->getDimensions();
	//Array2f lt = r->getLeftTop();
	//createTexts(lt, dims, f, f1, f2, lb, mb);
}

void DrawLabelledMessage::createTexts(Array2f lt, Vector2f dims, IDWriteFactory* f,
	IDWriteTextFormat* f1, IDWriteTextFormat* f2, ID2D1Brush* lb, ID2D1Brush* mb) {
	const Vector2f virtualDims(dims(0), dims(1) / 2);
	const Array2f messagePoint(lt(0), lt(1) + virtualDims(1));
	label = TextLabel(lt, virtualDims, f, f1, lb);
	message = TextLabel(messagePoint, virtualDims, f, f2, mb);
}

void DrawLabelledMessage::setLabel(wstring l) {
	label.setLabel(l);
}

void DrawLabelledMessage::setMessage(wstring m) {
	message.setLabel(m);
}

void DrawLabelledMessage::setPosition(Array2f pos) {
	rectangle.reposition(pos);
	Array2f messagePosition = Array2f(pos(0), pos(1) + label.getDimensions()(1));
	label.setLeftTopPosition(pos);
	message.setLeftTopPosition(messagePosition);
}

//void DrawLabelledMessage::setSize(Vector2f v) {
//	labelRectangle.setDimensions(v);
//	messageRectangle.setDimensions(v);
//}

void DrawLabelledMessage::draw(ID2D1HwndRenderTarget* rt) const {
	rectangle.fill(rt);
	label.draw(rt);
	message.draw(rt);
}

DrawLabelledMessage::~DrawLabelledMessage() {}


RectangleButton::RectangleButton() :
	RectangleButton(Array2f(0, 0), Vector2f(0, 0), nullptr) {};

RectangleButton::RectangleButton(Array2f lt, Vector2f dims, ID2D1Brush* rb)
	: DrawRectangle(lt, dims, rb), virtualButton(VirtualRectangle(lt, dims)), Button()
{}

void RectangleButton::setPosition(Array2f pos) {
	virtualButton.setLeftTopPosition(pos);
	DrawRectangle::reposition(pos);
}

bool RectangleButton::isInside(Array2f p) const {
	return virtualButton.isPointInside(p);
}


void RectangleButton::draw(ID2D1HwndRenderTarget* rt) const {
	DrawRectangle::draw(rt);
}

TextRectangleButton::TextRectangleButton() : TextRectangleButton(Array2f(0, 0), Vector2f(0, 0),
nullptr, nullptr, nullptr, nullptr) {

}

TextRectangleButton::TextRectangleButton(Array2f lt, Vector2f dims, ID2D1Brush* rb, IDWriteFactory* wf,
	IDWriteTextFormat* lf, ID2D1Brush* lb) 
: RectangleButton(lt, dims, rb), 
label(lt, dims, wf, lf, lb)
{};

void TextRectangleButton::setPosition(Array2f pos) {
	RectangleButton::setPosition(pos);
	label.setLeftTopPosition(pos);
}

void TextRectangleButton::setLabel(wstring l) {
	label.setLabel(l);
}

wstring TextRectangleButton::getLabel() const {
	return label.getLabel();
}

void TextRectangleButton::draw(ID2D1HwndRenderTarget* rt) const {
	RectangleButton::draw(rt);
	label.draw(rt);
}

void TextRectangleButton::fill(ID2D1HwndRenderTarget* rt) const {
	RectangleButton::fill(rt);
	label.draw(rt);
}

DraggableShape::DraggableShape() : DraggableShape(nullptr) {};

DraggableShape::DraggableShape(shared_ptr<DrawShape> obj) : drawShape(obj), isDragging(false) {
	if(obj){
		//virtualShape = obj->toVirtualShape();
	}
};

DraggableShape::DraggableShape(shared_ptr<VirtualShape> shape, ID2D1Brush* b) 
: virtualShape(shape), isDragging(false)
{
	if (shape && b) {
		//drawShape = shape->toDrawObject(b);
		//shape->toDrawObject(drawShape, b);
	}
}

bool DraggableShape::isInside(Array2f p) {
	if(virtualShape){
		isDragging = virtualShape->isPointInside(p);
		return isDragging;
	}
	isDragging = false;
	return false;
}

void DraggableShape::handleMouseDown(Array2f p) {
	isDragging = isInside(p);
}

void DraggableShape::handleMouseUp() {
	isDragging = false;
}

void DraggableShape::move(Vector2f v) {
	if(virtualShape){
		if(isDragging){
			virtualShape->translate(v);
			shared_ptr<VirtualRectangle> vr = dynamic_pointer_cast<VirtualRectangle>(virtualShape);
			//virtualShape->toDrawObject(drawShape, drawShape->getBrush());
		}
	}
};

void DraggableShape::draw(ID2D1HwndRenderTarget* rt) const {
	if(drawShape) drawShape->draw(rt);

};
shared_ptr<VirtualObject> DraggableShape::toVirtualObject() const{
	return virtualShape;
}


NewDragShape::NewDragShape() : NewDragShape(nullptr, nullptr){};

NewDragShape::NewDragShape(shared_ptr<VirtualShape> sh, ID2D1Brush* b) : isDragging(false), shape(sh) {
	if (shape) {
		shape->setBrush(b);
	}
}



//NewDragShape::NewDragShape(unique_ptr<VirtualShape> sh, ID2D1Brush* b) : isDragging(false), shape(move(sh))  {
//	if (shape) {
//		shape->setBrush(b);
//	}
//}
//
//unique_ptr<VirtualShape> NewDragShape::getShape() const {
//	return make_unique<VirtualShape>(shape);
//}



//NewDragShape::NewDragShape(VirtualShape* sh, ID2D1Brush* b) : shape(sh) {
//	if (shape) {
//		shape->setBrush(b);
//	}
//}
//
//VirtualShape* NewDragShape::getShape() const {
//	return shape;
//}

shared_ptr<VirtualShape> NewDragShape::getShape() const {
	return shape;
}

bool NewDragShape::isInside(Array2f p) {
	return shape->isPointInside(p);
}

void NewDragShape::handleMouseDown(Array2f p) {
	isDragging = isInside(p);
}
void NewDragShape::handleMouseUp() {
	isDragging = false;
}
bool NewDragShape::movement(Vector2f v) {
	if (shape && isDragging) {
		shape->translate(v);
		return true;
	}
	return false;
}

void NewDragShape::draw(ID2D1HwndRenderTarget* rt) const {
	shape->draw(rt);
}

void NewDragShape::fill(ID2D1HwndRenderTarget* rt) const {
	shape->fill(rt);
}

HoverShape::HoverShape() : HoverShape(nullptr){};

HoverShape::HoverShape(shared_ptr<VirtualShape> sh, ID2D1Brush* b) : shape(sh), inside(false){
	if(shape){
		shape->setBrush(b);
	}
}

void HoverShape::setFunctionIn(function<void()> f) {
	funcIn = f;
}

void HoverShape::setFunctionOut(function<void()> f) {
	funcOut = f;
}

void HoverShape::activate(Array2f position) {
	if (shape) {
		if(!inside && shape->isPointInside(position)){
			if(funcIn){
				funcIn();
				inside = true;
			}
		}
		else if(inside && !shape->isPointInside(position)){
			if (funcOut) {
				funcOut();
				inside = false;
			}
		}
	}
}

void HoverShape::draw(ID2D1HwndRenderTarget* rt) const{
	if (shape) {
		shape->draw(rt);
	}
};
void HoverShape::fill(ID2D1HwndRenderTarget* rt) const{
	if (shape) {
		shape->fill(rt);
	}
};

DropDownList::DropDownList() : DropDownList(Array2f(0, 0), Vector2f(100, 20), nullptr, nullptr, nullptr, nullptr){};

DropDownList::DropDownList(Array2f lt, Vector2f bd, ID2D1Brush* bb, 
IDWriteFactory* wf, IDWriteTextFormat* f, ID2D1Brush* tb)
: left_top(lt), button_dims(bd), back_brush(bb), write_factory(wf), button_font(f), text_brush(tb),
list(lt, bd, bb, wf, f, tb), is_open(false), buttons_rect(Array2f(lt(0), lt(1)+bd(1)), Vector2f(0, 0)) {
	function<void()> func = [&]() {
		if(!is_open){
			this->openList();
		}
	};
	list.setFunction(func);
	triangleOpenIndicator();
	/*
	float ttop = lt(1) + 0.3 * bd(1);
	float tleft = lt(0) + 0.85 * bd(0);
	float tright = lt(0) + 0.93 * bd(0);
	float tbottom = lt(1) + 0.7 * bd(1);
	list_indic = VirtualTriangle(Array2f(tleft, ttop), Array2f(tright, ttop), Array2f((tright-tleft)/2, tbottom), tb);*/

	//list.activate()
};

void DropDownList::triangleOpenIndicator() {
	float ttop = left_top(1) + (0.3 * button_dims(1));
	float tleft = left_top(0) + (0.88 * button_dims(0));
	float tright = left_top(0) + (0.96 * button_dims(0));
	float tbottom = left_top(1) + (0.7 * button_dims(1));
	list_indic = VirtualTriangle(Array2f(tleft, ttop), Array2f(tright, ttop), Array2f((tright + tleft) / 2, tbottom), text_brush);
}

void DropDownList::triangleCloseIndicator() {
	float ttop = left_top(1) + 0.3 * button_dims(1);
	float tleft = left_top(0) + 0.88 * button_dims(0);
	float tright = left_top(0) + 0.96 * button_dims(0);
	float tbottom = left_top(1) + 0.7 * button_dims(1);
	list_indic = VirtualTriangle(Array2f(tleft, tbottom), Array2f(tright, tbottom), Array2f((tright + tleft) / 2, ttop), text_brush);
}

void DropDownList::setSelected(wstring sel) {
	list.setLabel(sel);
}

void DropDownList::setFunction(function<void(unsigned int)> f) {
	func = f;
}

unsigned int DropDownList::addButton(wstring text, function<void()> func) {
	unsigned int butt_id = drop_buttons.size();
	Array2f new_lt = {left_top(0), left_top(1) + (button_dims(1) * (drop_buttons.size() + 1))};
	unique_ptr<TextRectangleButton> trb = make_unique<TextRectangleButton>(new_lt, button_dims, back_brush, write_factory, button_font, text_brush);
	trb->setFunction(func);
	trb->setLabel(text);
	drop_buttons.push_back(move(trb));
	//Vector2f but_dim = list..getDimensions();
	buttons_rect.setDimensions(Vector2f(button_dims(0), drop_buttons.size() * button_dims(1)));
	return butt_id;
}

void DropDownList::activate(Array2f position) {
	//list.activate(position);
	if (is_open) {
		if(buttons_rect.isPointInside(position)){
			unsigned int butt_id = floor((position(1) - buttons_rect.getLeftTop()(1)) / button_dims(1));
			//outputDebugLine(to_string(butt_id));
			drop_buttons[butt_id]->activate(position);
			setSelected(drop_buttons[butt_id]->getLabel());
			if(func) func(butt_id);
		}
		closeList();
	}
	else {
		//openList();
		if (list.isInside(position)) {
			openList();
		}
	}
}

void DropDownList::openList() {
	is_open = true;
	triangleCloseIndicator();
}

void DropDownList::closeList() {
	is_open = false;
	triangleOpenIndicator();
}

void DropDownList::clearList() {
	drop_buttons.clear();
	buttons_rect.setDimensions(Vector2f());
}

void DropDownList::draw(ID2D1HwndRenderTarget* rt) const {
	list.fill(rt);
	list_indic.fill(rt);
	if (is_open) {
		//outputDebugLine("op");
		for (unique_ptr<TextRectangleButton> const& trb: drop_buttons) {
			trb->fill(rt);
		}
	}
}

Arrow::Arrow() : Arrow(Array2f(), Vector2f()) {};

Arrow::Arrow(Array2f p1, Array2f p2, float tw, float hs, ID2D1Brush* b) :// brush(b),
tail(DrawLine(p1, p2, b, tw)), line(p1, p2) {
	
	Vector2f v(p2(0)-p1(0), p2(1)-p1(1));
	v.normalize();
	Array2f n(-v(1), v(0));
	v = v * hs;
	n = n * hs;
	Array2f t1(p2(0) + n(0), p2(1) + n(1));
	Array2f t2(p2(0) - n(0), p2(1) - n(1));
	Array2f t3(p2(0) + v(0), p2(1) + v(1));
	head = VirtualTriangle(t1, t2, t3, b);
}

Arrow::Arrow(Array2f p, Vector2f v, float tw, float hs, ID2D1Brush* b) : 
Arrow(p, Array2f(p(0) + v(0), p(1) + v(1)), tw, hs, b){}

Arrow::Arrow(Array2f p1, Array2f p2, ID2D1Brush* b) : Arrow(p1, p2, 4, 6, b){};

Array2f Arrow::getHeadPoint() const {
	return line.getStart();
}

Array2f Arrow::getTailPoint() const {
	return line.getEnd();
}

void Arrow::setBrush(ID2D1Brush* b) {
	tail.setBrush(b);
	head.setBrush(b);
	//brush = b;
}

void Arrow::draw(ID2D1HwndRenderTarget* rt) const {
	//if (brush) {
		tail.draw(rt);
		head.fill(rt);
	//}
}

Slider::Slider() : Slider(Array2f(0, 0), 10, Vector2f(2, 2)){};

Slider::Slider(Array2f pos, float sw, Vector2f dims, 
ID2D1Brush* nb, ID2D1Brush* ab, ID2D1Brush* lb) : position(pos), slide_width(sw),
normal_brush(nb), activated_brush(ab), value(0), activated(false) {
	Vector2f hdim = dims / 2;
	const Array2f left_top(pos(0) - hdim(0), pos(1) - hdim(1));
	rectangle_handle = VirtualRectangle(left_top, dims, nb);
	slide_line = VirtualLine(pos, Vector2f(sw, 0), lb);
}

void Slider::setChangeValueFunction(function<void(float)> func) {
	on_value_change = func;
}

void Slider::handleMouseMove(Array2f position) {
	if (activated) {
		Vector2f hdim = rectangle_handle.getDimensions() / 2;
		Array2f start = slide_line.getStart();
		Array2f end = slide_line.getEnd();
		Array2f lt(position(0), start(1));
		if (position(0) < start(0)) {
			lt(0) = start(0);
		}
		else if(position(0) > end(0)) {
			lt(0) = end(0);
		}
		rectangle_handle.setLeftTopPosition(Array2f(lt(0)-hdim(0), lt(1)-hdim(1)));
		value = (lt(0)-start(0))/slide_width;
		if(on_value_change) on_value_change(value);
	}
}


void Slider::handleMouseDown(Array2f position) {
	if (rectangle_handle.isPointInside(position)) {
		activated = true;
		rectangle_handle.setBrush(activated_brush);
	}
}

void Slider::handleMouseUp(Array2f position) {
	activated = false;
	rectangle_handle.setBrush(normal_brush);
}

void Slider::draw(ID2D1HwndRenderTarget* rt) const {
	slide_line.draw(rt);
	rectangle_handle.fill(rt);
}