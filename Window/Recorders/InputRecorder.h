#pragma once

#include <vector>
#include <windows.h>
#include <bitset>
#include <chrono>

#include <Eigen/Dense>

#include "../Controllers/InputController.h"
#include "../Util.h"
#include "TimerManager.h"
#include "Updater.h"

using namespace std;
using namespace Eigen;

//class 

class MouseRecord{
private:
	unsigned int elapsedTime; // in ms? maybe micro time (unit to be determined)
	Vector2f movement;
	Array2i position; // absolute position of the mouse
	//Array2i absolutePosition;
	//MouseState mouseState;
	MouseAction mouseAction;

public:
	MouseRecord();
	MouseRecord(Vector2f mt, Array2f pos, Array2i absPos, unsigned int );
	//MouseRecord(Array2i pos, MouseState ms=MouseState());
	MouseRecord(Array2i pos, MouseAction ma);
	//MouseState getMouseState() const;
	MouseAction getMouseAction() const;
	Vector2f getMovement() const;
	//Array2i getAbsolutePosition() const;
	float getAverageSpeed() const;
	Array2i getPosition() const;
	void play(shared_ptr<MouseAutoInput> ai);
};

class KeyRecord {
private:
	//vector<unsigned int> keys_down;
	//vector<unsigned int> keys_up;
	KeyAction key_action;
public:
	KeyRecord();
	KeyRecord(KeyAction ka);
	unordered_set<unsigned int> getKeysDown() const;
	void addKeyDown(unsigned int k);
	unordered_set<unsigned int> getKeysUp() const;
	void addKeyUp(unsigned int k);
	void play(shared_ptr<KeyboardAutoInput> ai);
};

class InputRecord : public MouseRecord, public KeyRecord {
public:
	InputRecord(Array2i pos, MouseAction ma, KeyAction ka);
	void play(shared_ptr<AutoInput> ai);
};

class MouseClickRecord {
private:
	
public:
};

class MouseRecorder : public Updater {
private:
	shared_ptr<TimerManager> timerManager;
	shared_ptr<MouseController> mouseController;
	//Array2f lastPosition;
	bool isRecording;
	//unique_ptr<unsigned int> timerIdentifier;

	vector<MouseRecord> recordStream;
	MouseAction currentMouseAction;



	//unsigned int startingTime;
	Array2f startingPosition;
	Array2i absStartingPosition;
	unsigned int totalTime;
	float distance;

	TimerFunction mouseMovementRecorder;

	//MouseState currState;
	//chrono::steady_clock::time_point startClock;
	//QueueTimeEvents leftClickDownEvents;
	//QueueTimeEvents leftClickUpEvents;
	//vector<unsigned int> leftClickDownTimes;
	//vector<unsigned int> leftClickUpTimes;

public:
	//MouseRecorder();
	MouseRecorder(shared_ptr<MouseController> mc, shared_ptr<TimerManager> tm);

	void start(unsigned int interval);
	void end();

	void update();

	void addRecord(Vector2f movement, Array2f pos, Array2i absPos, unsigned int time);
	void addRecord(Array2i pos, MouseState ms=MouseState());
	void addRecord(Array2i pos, MouseAction ma=MouseAction());
	MouseRecord getRecord(unsigned int rn) const;
	Array2f getStartingPosition() const;
	vector<MouseRecord> getRecordStream() const;

	void addLeftClickDown();
	void addLeftClickUp();
	void addRightClickDown();
	void addRightClickUp();
	void addKeyDown(unsigned int k);
	void addKeyUp(unsigned int k);

	//vector<unsigned int> getLeftClicksDown() const;
	//vector<unsigned int> getLeftClicksUp() const;
	vector<unsigned int> geRightClicksDown() const;

	bool getIsRecording() const;

	float getAverageSpeed() const;
	float getDistance() const;
};

class MouseRecordPlayer : public Updater {
private:
	shared_ptr<TimerManager> timerManager;
	shared_ptr<MouseAutoInput> mouseAutoInput;
	//shared_ptr<AutoInput> autoInput;
	unsigned int recordIndex;

	shared_ptr<MouseRecorder> mouseRecorder;

	VectorStream<MouseRecord> mousePointData;
	VectorStream<InputRecord> inputRecordData;
	bool isPlaying;
	bool isFinished;
	//VectorStream<

	TimerFunction mouseMoveTimerEvent;
	QueuedTimer leftClickDownQueue;
	QueuedTimer leftClickUpQueue;
	QueuedTimer rightClickDownQueue;

public:
	MouseRecordPlayer(shared_ptr<TimerManager> tm, 
	shared_ptr<MouseAutoInput> mai, vector<MouseRecord> mrs=vector<MouseRecord>());
	void setMouseRecorder(shared_ptr<MouseRecorder> mr);
	//void setRecordStream(vector<MouseRecord> mrs);
	bool getIsPlaying() const;
	bool play(unsigned int interval); // returns isPlaying
	bool next();
	void pause();
	void stop();

	void update();
};

class InputRecorder : public Updater {
private:
	//shared_ptr<MouseController> mouseController;
	vector<InputRecord> records;
	MouseAction currentMouseAction;
	KeyAction currentKeyAction;
	bool isRecording;
	bool isPaused;
public:
	InputRecorder();
	void start();
	void pause();
	void end();

	vector<InputRecord> getRecords() const;
	VectorStream<InputRecord> getRecordStream() const;

	void addRecord(Array2i pos, MouseAction ma = MouseAction(), KeyAction ka = KeyAction());
	void addMouseLeftDown();
	void addMouseLeftUp();
	void addMouseRightDown();
	void addMouseRightUp();
	void addKeyDown(unsigned int k);
	void addKeyUp(unsigned int k);

	void update();
};

class InputRecordPlayer : public Updater {
private:
	shared_ptr<AutoInput> autoInput;
	//shared_ptr<InputRecorder> recorder;

	VectorStream<InputRecord> recordStream;
	bool isPlaying;
	bool isFinished;
public:
	InputRecordPlayer(shared_ptr<AutoInput> ai);
	bool getIsPlaying() const;

	void setRecord(VectorStream<InputRecord> rec);
	bool play();
	bool next();
	void pause();
	void stop();

	void update();
};