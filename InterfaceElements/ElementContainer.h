#pragma once

#include <vector>
#include <memory>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "BasicElements.h"

using namespace std;

class ButtonManager {
private:
	vector<shared_ptr<Button>> buttons;
public:
	ButtonManager();
	void addButton(shared_ptr<Button> b);
	void setButtons(vector<shared_ptr<Button>> bs);
	void clearButtons();
	void activate(Array2f position);
	void draw(ID2D1HwndRenderTarget* rt);
};