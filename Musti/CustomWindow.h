#pragma once

#include <functional>

#include "AdvancedWindow.h"
#include "../Geometry/Util.h"
#include "../Geometry/Containers.h"
#include "../Window/Recorders/InputRecorder.h"
#include "../InterfaceElements/ElementContainer.h";
#include "../InterfaceElements/BasicElements.h";

using namespace Eigen;
using namespace std;

class MyApp;

class CustomTest : public ControllersWindow {
private:
	//Canvas canvas;
	unique_ptr<MyApp> app;

	HRESULT createResources();
	void discardResources();
	void drawFrame();
	void handleMouseMove(WPARAM wParam, LPARAM lParam);
	void handleLeftMouseDown(WPARAM wParam, LPARAM lParam);
	void handleLeftMouseUp(WPARAM wParam, LPARAM lParam);
	void handleRightMouseDown(WPARAM wParam, LPARAM lParam);
public:
	CustomTest();
	CustomTest(int width, int height);
	LPCWSTR className() const { return L"Controllers Window"; }
	LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class MyApp : public Canvas {
private:
	ID2D1Brush* b1;
	ID2D1Brush* b2;
	bool isB1;

	DrawRectangle rectangle;
	DraggableShape ds;
	shared_ptr<TextRectangleButton> myButton;
	shared_ptr<TextRectangleButton> endButton;
	shared_ptr<MouseRecorder> mouseRecorder;
	shared_ptr<Button> but;

public:
	MyApp();
	void addRectangle(DrawRectangle dr);
	void addButton(TextRectangleButton mb, bool isEnd=false);
	void addButton(shared_ptr<TextRectangleButton> mb, bool isEnd = false);
	void addB(shared_ptr<Button> b);
	void setBrushes(ID2D1Brush* a, ID2D1Brush* b);
	void changeColour();
	//void setDraggable(DraggableShape d);
	void newGoal();
	void startRecord();
	
	void handleMouseMove(Vector2f movement);
	void handleMouseClick(Array2f point);
	void handleMouseUp(Array2f point);

	void draw(ID2D1HwndRenderTarget* rt);
};

class AppTest : public BaseApp {
private:
	DrawRectangle rect;
	DraggableShape ds;
	DrawLabelledMessage dlm;
	DrawLabelledMessage dlm2;
	DrawLabelledMessage dlm3;
	shared_ptr<TextRectangleButton> playButton;
	//bool 
	shared_ptr<TextRectangleButton> start;
	shared_ptr<TextRectangleButton> end;
	ButtonManager bm;

	shared_ptr<ExternalWindows> externalWindows;

	DrawLabelledMessage dlm4;
	unsigned int numClicked;

	void clickMessage();

	//Can
	//shared_ptr<VirtualRectangle> vir
	shared_ptr<VirtualRectangle> connRectangle;
	//DrawShapeVir drawShapeVir;
	//shared_ptr<VirtualRectangle> dragRect;
	shared_ptr<VirtualRectangle> dragRect;
	NewDragShape nds;

	DropDownList list;

	SimpleGeometry sg;


	Array2f p1;
	Array2f p2;
	//DrawEllipse line_sta;
	//DrawEllipse line_end;
	DrawLine dl;
	VirtualLine virt_line;

	shared_ptr<VirtualCircle> line_sta;
	shared_ptr<VirtualCircle> line_end;
	NewDragShape circ_sta;
	NewDragShape circ_end;

	VirtualTriangle test_tri;

	VirtualRectangle monitor;
	shared_ptr<VirtualRectangle> small_window;
	NewDragShape window_drag;

	shared_ptr<MouseRecorder> mouseRec;
	shared_ptr<InputRecorder> inputRecorder;
	shared_ptr<MouseRecordPlayer> mousePlayer;
	shared_ptr<InputRecordPlayer> inputPlayer;
	QueuedTimer qt;

	void test();
	static void __stdcall t(void* param, bool b);
public:
	AppTest();
	void createResources();
	VirtualRectangle updateObjects();
	void drawFrame(ID2D1HwndRenderTarget* rt);
	void playPause();
	void handleKeyDown(unsigned int keyCode, char c);
	void handleKeyUp(unsigned int keyCode, char c);
	void handleMouseMove(Array2f position, Vector2f movement);
	void handleLeftMouseDown(Array2f position);
	void handleLeftMouseUp(Array2f position);
	void handleRightMouseDown(Array2f position);
	//void handleRightMouseUp(Array2f position);
};