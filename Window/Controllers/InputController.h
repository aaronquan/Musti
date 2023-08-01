#pragma once
#include <windows.h>
#include <windowsx.h>
#include <unordered_set>
#include <string>
#include <unordered_map>
#include <memory>
#include <iterator>
#include <bitset>

#include "../Util.h"
#include "../WindowProperties.h"

#include "../../Geometry/Primatives.h"

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

using KeyInput = unsigned int;

//constexpr map<WPARAM, string> hexKeyToStringKey = ;

//reason why some held keys don't fully hold, hardware limitation e.g. A W 1 2 <- doesn't detect
// max keys = 6 

class MouseState {
private:
	bitset<4> state_;
public:
	MouseState();
	bool getLeftClick() const;
	void setLeftClick(bool b);

	bool getRightClick() const;
	void setRightClick(bool b);
};

class MouseAction {
private:
	bitset<4> state_;
public:
	MouseAction();
	bool getLeftClickDown() const;
	void setLeftClickDown(bool b);
	bool getLeftClickUp() const;
	void setLeftClickUp(bool b);
	bool getRightClickDown() const;
	void setRightClickDown(bool b);
	bool getRightClickUp() const;
	void setRightClickUp(bool b);
};

class KeyState {
private:
	unordered_set<unsigned int> keys_down;
public:
	KeyState();
	void addKeyDown(unsigned int k);
	void removeKeyDown(unsigned int k);
	bool hasKeyDown(unsigned int k) const;
	unordered_set<unsigned int> getKeysDown() const;
};

class KeyAction : public KeyState {
private:
	unordered_set<unsigned int> keys_up;
public:
	KeyAction();
	void addKeyUp(unsigned int k);
	void removeKeyUp(unsigned int k);
	bool hasKeyUp(unsigned int k) const;
	unordered_set<unsigned int> getKeysUp() const;
};

class KeyboardController {
protected:
	unordered_set<unsigned int> all_keys_down; // other keys such as tab 
	KeyState key_state;
	unordered_set<char> print_keys_down; // printable keys such as c, d, e
	//static const unordered_map<WPARAM, string> otherKeyMap;

public:
	KeyboardController();
	char asciiKeyDown(WPARAM wParam, LPARAM lParam);
	char asciiKeyUp(WPARAM wParam, LPARAM lParam);
	char onKeyPress(WPARAM wParam, LPARAM lParam);
	char onKeyRelease(WPARAM wParam, LPARAM lParam);
	bool isPrintKeyDown(char pk);
	unordered_set<unsigned int> getKeysDown();
	unordered_set<char> getPrintKeysDown();

	static unique_ptr<char> keyAsChar(KeyInput k);
	static char keyAsAscii(WPARAM wParam, LPARAM lParam);
};

class MouseController {
protected:
	unordered_set<unsigned int> mouseButtonsDown;
	VirtualPoint2F lastMousePosition;
	VirtualPoint2F mousePosition;

	VirtualPoint2I absoluteMousePosition;

	MouseState mouseState;

public:
	MouseController();
	VirtualVector2F getMouseMovement() const;
	VirtualPoint2F getMousePosition() const;
	VirtualPoint2F getLastMousePosition() const;
	MouseState getMouseState() const;
	void onLeftMouseDown(WPARAM wParam, LPARAM lParam);
	void onLeftMouseUp(WPARAM wParam, LPARAM lParam);
	void onRightMouseDown(WPARAM wParam, LPARAM lParam);
	void onRightMouseUp(WPARAM wParam, LPARAM lParam);
	void onMouseMove(WPARAM wParam, LPARAM lParam);
};

class InputController : public KeyboardController, public MouseController {
private:
	//KeyboardController kc;
	//MouseController mc;
public:
	InputController();
};

//can implement class for exact timer input


template <typename E>
class InputBinder : public InputController {
private:
	unordered_map<unsigned int, E> inputActionBinder;
	//enum actions;
public:
	InputBinder() {};
	void bindKey(unsigned int ui, E e) {
		inputActionBinder.insert(std::make_pair(ui, e));
	};
	void unbindKey(unsigned int ui) {
		inputActionBinder.erase(ui);
	};
	unique_ptr<E> getAction(unsigned int ui) {
		typename unordered_map<unsigned int, E>::iterator f = inputActionBinder.find(ui);
		if(f == inputActionBinder.end()) return unique_ptr<E>(nullptr);
		return make_unique<E>(f->second);
	};
};

//outputs mouse output

class MouseAutoInput {
protected:
	shared_ptr<MouseController> mouseController;
	shared_ptr<WindowDetails> windowDetails;
	
	static void mouseInput(MOUSEINPUT mi);
public:
	//MouseAutoInput();
	MouseAutoInput(shared_ptr<MouseController> mc=nullptr, shared_ptr<WindowDetails> wd=nullptr);
	void addMouseController(shared_ptr<MouseController> mc);
	void addWindowDetails(shared_ptr<WindowDetails> wd);

	void mouseMove(Vector2f movement);
	void mouseWindowPosition(Array2f position);
	void mouseGlobalPosition(Array2f position);
	void mouseGlobalPosition(Array2i position);
	void mouseLeftClickDown(Array2i position);
	void mouseLeftClickDown();
	void mouseLeftClickUp();
	void mouseRightClickDown(Array2i position);
	void mouseRightClickDown();
	void mouseRightClickUp();
};

class KeyboardAutoInput {
protected:
	shared_ptr<KeyboardController> keyboardController;
	static void keyInput(KEYBDINPUT ki);
public:
	KeyboardAutoInput(shared_ptr<KeyboardController> kc=nullptr);
	void addKeyboardAutoInput(shared_ptr<KeyboardController> kc);
	void keyDown(unsigned int keyCode);
	void keyUp(unsigned int keyCode);
};

class AutoInput : public MouseAutoInput, public KeyboardAutoInput {
public:
	AutoInput();
	void addInputController(shared_ptr<InputController> ic);
};