#pragma once
#include "InputController.h"

using namespace std;

//const unordered_map<WPARAM, string> KeyboardController::otherKeyMap = {
	//{VK_CONTROL}
//};

MouseState::MouseState() {};

bool MouseState::getLeftClick() const {
	return state_[0];
}

void MouseState::setLeftClick(bool b) {
	state_[0] = b;
}

bool MouseState::getRightClick() const {
	return state_[1];
}

void MouseState::setRightClick(bool b) {
	state_[1] = b;
}

MouseAction::MouseAction(){};

bool MouseAction::getLeftClickDown() const {
	return state_[0];
}
void MouseAction::setLeftClickDown(bool b) {
	state_[0] = b;
}
bool MouseAction::getLeftClickUp() const {
	return state_[1];
}
void MouseAction::setLeftClickUp(bool b) {
	state_[1] = b;
}
bool MouseAction::getRightClickDown() const {
	return state_[2];
}
void MouseAction::setRightClickDown(bool b) {
	state_[2] = b;
}
bool MouseAction::getRightClickUp() const {
	return state_[3];
}
void MouseAction::setRightClickUp(bool b) {
	state_[3] = b;
}

KeyState::KeyState() {};

void KeyState::addKeyDown(unsigned int k) {
	keys_down.insert(k);
}

void KeyState::removeKeyDown(unsigned int k) {
	keys_down.erase(k);
}

bool KeyState::hasKeyDown(unsigned int k) const {
	return keys_down.find(k) != keys_down.end();
}

unordered_set<unsigned int> KeyState::getKeysDown() const {
	return keys_down;
}

KeyAction::KeyAction() {};
void KeyAction::addKeyUp(unsigned int k) {
	keys_up.insert(k);
}
void KeyAction::removeKeyUp(unsigned int k) {
	keys_up.erase(k);
}
bool KeyAction::hasKeyUp(unsigned int k) const {
	return keys_up.find(k) != keys_up.end();
}
unordered_set<unsigned int> KeyAction::getKeysUp() const{
	return keys_up;
}

KeyboardController::KeyboardController() {};
/*
unique_ptr<char> KeyboardController::keyAsChar(KeyInput key) {
	unsigned int k = MapVirtualKeyW(key, MAPVK_VK_TO_CHAR);
	char c = LOWORD(k);
	if (!isprint(c)) nullptr;
	return make_unique<char>(c);
}
*/
char KeyboardController::asciiKeyDown(WPARAM wParam, LPARAM lParam) {
	char ascii = keyAsAscii(wParam, lParam);
	print_keys_down.insert(ascii);
	return ascii;
}
char KeyboardController::asciiKeyUp(WPARAM wParam, LPARAM lParam) {
	char ascii = keyAsAscii(wParam, lParam);
	print_keys_down.erase(ascii);
	return ascii;
}
char KeyboardController::onKeyPress(WPARAM wParam, LPARAM lParam) {
	char ascii = asciiKeyDown(wParam, lParam);
	all_keys_down.insert(wParam);
	key_state.addKeyDown(wParam);
	return ascii;
}
char KeyboardController::onKeyRelease(WPARAM wParam, LPARAM lParam) {
	char ascii = asciiKeyUp(wParam, lParam);
	all_keys_down.erase(wParam);
	key_state.removeKeyDown(wParam);
	outputDebugLine(wParam);
	return ascii;
}

bool KeyboardController::isPrintKeyDown(char pk) {
	return key_state.hasKeyDown(pk);
}
unordered_set<unsigned int> KeyboardController::getKeysDown() {
	return key_state.getKeysDown();
}
unordered_set<char> KeyboardController::getPrintKeysDown() {
	return print_keys_down;
}
char KeyboardController::keyAsAscii(WPARAM wParam, LPARAM lParam) {
	unsigned char keys[256];
	WORD ch;
	bool b = GetKeyboardState(keys);
	ToAscii(wParam, lParam, keys, &ch, 0);
	//string chOut;
	//chOut.push_back(static_cast<char>(toupper(ch)));
	//outputDebugLine(chOut.c_str());
	return static_cast<char>(toupper(ch));
}

MouseController::MouseController() : mousePosition(0, 0), lastMousePosition(0, 0) {};

VirtualVector2F MouseController::getMouseMovement() const {
	return VirtualPoint2F::difference(lastMousePosition, mousePosition);
}

VirtualPoint2F MouseController::getLastMousePosition() const {
	return lastMousePosition;
}

VirtualPoint2F MouseController::getMousePosition() const {
	return mousePosition;
}

MouseState MouseController::getMouseState() const {
	return mouseState;
}

void MouseController::onLeftMouseDown(WPARAM wParam, LPARAM lParam) {
	mouseState.setLeftClick(true);
}

void MouseController::onLeftMouseUp(WPARAM wParam, LPARAM lParam) {
	mouseState.setLeftClick(false);
}

void MouseController::onRightMouseDown(WPARAM wParam, LPARAM lParam) {
	mouseState.setRightClick(true);
}

void MouseController::onRightMouseUp(WPARAM wParam, LPARAM lParam) {
	mouseState.setRightClick(false);
}


void MouseController::onMouseMove(WPARAM wParam, LPARAM lParam){
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	lastMousePosition = mousePosition;
	mousePosition = Array2f(xPos, yPos);
	//stringstream ss;
	//ss << mousePosition;
	//outputDebugLine(ss.str());
};


InputController::InputController(){};

MouseAutoInput::MouseAutoInput(shared_ptr<MouseController> mc, shared_ptr<WindowDetails> wd)
: mouseController(mc), windowDetails(wd){};

void MouseAutoInput::addMouseController(shared_ptr<MouseController> mc) {
	mouseController = mc;
}

void MouseAutoInput::addWindowDetails(shared_ptr<WindowDetails> wd) {
	windowDetails = wd;
}

void MouseAutoInput::mouseInput(MOUSEINPUT mi) {
	INPUT input;
	input.mi = mi;
	input.type = INPUT_MOUSE;
	SendInput(1, &input, sizeof(input));
}

void MouseAutoInput::mouseMove(Vector2f movement) {
	MOUSEINPUT mi = { movement(0), movement(1), 0, MOUSEEVENTF_MOVE, 0 };
	INPUT input;
	//std::memset(&input, 0, sizeof(input));
	input.mi = mi;
	input.type = INPUT_MOUSE;
	SendInput(1, &input, sizeof(input));
}

void MouseAutoInput::mouseWindowPosition(Array2f position) {
	if(mouseController && windowDetails){
		Array2f windowCorner = windowDetails->getLeftTop() + position;
		Array2i absPos = windowDetails->toAbsolutePosition(windowCorner);
		MOUSEINPUT mi = { absPos(0), absPos(1), 0, MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, 0 };
		mouseInput(mi);
	}
}

void MouseAutoInput::mouseGlobalPosition(Array2f position) {
	if (mouseController && windowDetails) {
		Array2i absPos = windowDetails->toAbsolutePosition(position);
		MOUSEINPUT mi = { absPos(0), absPos(1), 0, MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, 0 };
		mouseInput(mi);
	}
}

void MouseAutoInput::mouseGlobalPosition(Array2i position) {
	if (mouseController && windowDetails) {
		Array2i absPos = windowDetails->toAbsolutePosition(position);
		MOUSEINPUT mi = { absPos(0), absPos(1), 0, MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, 0 };
		mouseInput(mi);
	}
}

void MouseAutoInput::mouseLeftClickDown(Array2i position) {
	MOUSEINPUT mi = { position(0), position(1), 0, MOUSEEVENTF_LEFTDOWN, 10 };
	INPUT input;
	input.mi = mi;
	input.type = INPUT_MOUSE;
	SendInput(1, &input, sizeof(input));
}

void MouseAutoInput::mouseLeftClickDown() {
	MOUSEINPUT mi = { 0, 0, 0, MOUSEEVENTF_LEFTDOWN, 0 };
	INPUT input;
	input.mi = mi;
	input.type = INPUT_MOUSE;
	SendInput(1, &input, sizeof(input));
}

void MouseAutoInput::mouseLeftClickUp() {
	MOUSEINPUT mi = { 0, 0, 0, MOUSEEVENTF_LEFTUP, 0 };
	INPUT input;
	input.mi = mi;
	input.type = INPUT_MOUSE;
	SendInput(1, &input, sizeof(input));
}

void MouseAutoInput::mouseRightClickDown(Array2i position) {
	MOUSEINPUT mi = { position(0), position(1), 0, MOUSEEVENTF_RIGHTDOWN, 10 };
	INPUT input;
	input.mi = mi;
	input.type = INPUT_MOUSE;
	SendInput(1, &input, sizeof(input));
}

void MouseAutoInput::mouseRightClickDown() {
	MOUSEINPUT mi = { 0, 0, 0, MOUSEEVENTF_RIGHTDOWN, 10 };
	mouseInput(mi);
}

void MouseAutoInput::mouseRightClickUp() {
	MOUSEINPUT mi = { 0, 0, 0, MOUSEEVENTF_RIGHTUP, 10 };
	mouseInput(mi);
}

KeyboardAutoInput::KeyboardAutoInput(shared_ptr<KeyboardController> kc) : keyboardController(kc) {};

void KeyboardAutoInput::addKeyboardAutoInput(shared_ptr<KeyboardController> kc) {
	keyboardController = kc; 
}

void KeyboardAutoInput::keyInput(KEYBDINPUT ki) {
	INPUT input;
	input.ki = ki;
	input.type = INPUT_KEYBOARD;
	SendInput(1, &input, sizeof(input));
}

void KeyboardAutoInput::keyDown(unsigned int keyCode) {
	KEYBDINPUT ki = { keyCode, 0, 0, 0, 0 };
	keyInput(ki);
}

void KeyboardAutoInput::keyUp(unsigned int keyCode) {
	KEYBDINPUT ki = { keyCode, 0, KEYEVENTF_KEYUP, 0, 0 };
	keyInput(ki);
}

AutoInput::AutoInput() : MouseAutoInput(), KeyboardAutoInput() {};

void AutoInput::addInputController(shared_ptr<InputController> ic) {
	mouseController = ic;
	keyboardController = ic;
};