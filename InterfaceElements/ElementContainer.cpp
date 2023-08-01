#pragma once

#include "ElementContainer.h"

ButtonManager::ButtonManager()
{
}

void ButtonManager::addButton(shared_ptr<Button> b) {
	if(b) buttons.push_back(b);
}

void ButtonManager::setButtons(vector<shared_ptr<Button>> bs) {
	buttons = bs;
}

void ButtonManager::clearButtons() {
	buttons.clear();
}
void ButtonManager::activate(Array2f position) {
	for (shared_ptr<Button> b : buttons) {
		b->activate(position);
	}
}
void ButtonManager::draw(ID2D1HwndRenderTarget* rt) {
	for (shared_ptr<Button> b : buttons) {
		b->draw(rt);
	}
}