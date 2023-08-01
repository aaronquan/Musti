#pragma once

#include "InputRecorder.h"

//TIMERPROC Timerproc;

VOID CALLBACK Timerproc(
	HWND unnamedParam1,
	UINT unnamedParam2,
	UINT_PTR unnamedParam3,
	DWORD unnamedParam4
) {
	outputDebugLine("hi");
}

MouseRecord::MouseRecord() : MouseRecord(Vector2f(), Array2f(), Array2i(), 1) {}

MouseRecord::MouseRecord(Vector2f mt, Array2f pos, Array2i absPos, unsigned int time) : movement(mt), elapsedTime(time) {
	if(elapsedTime == 0) elapsedTime = 1; 
}

//MouseRecord::MouseRecord(Array2i pos, MouseState ms) : position(pos), elapsedTime(1), mouseState(ms){}

MouseRecord::MouseRecord(Array2i pos, MouseAction ma) : position(pos), elapsedTime(1), mouseAction(ma) {}

/*MouseState MouseRecord::getMouseState() const {
	return mouseState;
}*/

MouseAction MouseRecord::getMouseAction() const {
	return mouseAction;
}

Vector2f MouseRecord::getMovement() const {
	return movement;
}

//Array2i MouseRecord::getAbsolutePosition() const {
//	return absolutePosition;
//}

float MouseRecord::getAverageSpeed() const {
	return movement.norm()/elapsedTime; // runtime crash on 0 elapsed time
}

Array2i MouseRecord::getPosition() const {
	return position;
}

void MouseRecord::play(shared_ptr<MouseAutoInput> ai) {
	ai->mouseGlobalPosition(position);
	if (mouseAction.getLeftClickDown()) {
		ai->mouseLeftClickDown();
	}
	if (mouseAction.getLeftClickUp()) {
		ai->mouseLeftClickUp();
	}
	if (mouseAction.getRightClickDown()) {
		ai->mouseRightClickDown();
	}
	if (mouseAction.getRightClickUp()) {
		ai->mouseRightClickUp();
	}
}

KeyRecord::KeyRecord(){};

KeyRecord::KeyRecord(KeyAction ka) : key_action(ka){};

unordered_set<unsigned int> KeyRecord::getKeysDown() const {
	return key_action.getKeysDown();
}

void KeyRecord::addKeyDown(unsigned int k) {
	key_action.addKeyDown(k);
}

unordered_set<unsigned int> KeyRecord::getKeysUp() const {
	return key_action.getKeysUp();
}

void KeyRecord::addKeyUp(unsigned int k) {
	key_action.addKeyUp(k);
}

void KeyRecord::play(shared_ptr<KeyboardAutoInput> ai) {
	for (unsigned int key : key_action.getKeysDown()) {
		ai->keyDown(key);
	}
	for (unsigned int key : key_action.getKeysUp()) {
		ai->keyUp(key);
	}
}

InputRecord::InputRecord(Array2i pos, MouseAction ma, KeyAction ka) : MouseRecord(pos, ma), KeyRecord(ka) {};

void InputRecord::play(shared_ptr<AutoInput> ai) {
	MouseRecord::play(ai);
	KeyRecord::play(ai);
}

MouseRecorder::MouseRecorder(shared_ptr<MouseController> mc, shared_ptr<TimerManager> tm)
: timerManager(tm), mouseController(mc), isRecording(false), recordStream(), startingPosition(0, 0), 
totalTime(0), distance(0)
{}

void MouseRecorder::addRecord(Vector2f movement, Array2f pos, Array2i absPos, unsigned int time) {
	MouseRecord newRecord(movement, pos, absPos, time);
	recordStream.push_back(newRecord);
	totalTime += time;
	distance += movement.norm();
}

void MouseRecorder::addRecord(Array2i pos, MouseState ms) {
	//MouseState 
	//MouseRecord newRecord(pos);
	//recordStream.push_back(newRecord);
}

void MouseRecorder::addRecord(Array2i pos, MouseAction ma) {
	MouseRecord newRecord(pos, ma);
	recordStream.push_back(newRecord);
}

void MouseRecorder::start(unsigned int interval) {
	if(!isRecording){
		outputDebugLine("start recording");
		recordStream.clear();
		outputDebugLine(to_string(recordStream.size()));

		//startClock = chrono::steady_clock::now();
		Array2f position = mouseController->getMousePosition();
		//lastPosition = position;
		POINT p;
		GetCursorPos(&p);
		//startingPosition = position;
		startingPosition = Array2f(p.x, p.y);

		//leftClickDownEvents.start();
		//leftClickUpEvents.start();
		/*
		timerIdentifier = make_unique<unsigned int>(timerManager->addTimer([this, interval]() {
			//const Array2f newPosition = mouseController->getMousePosition();
			//Vector2f move = newPosition - lastPosition;
			//lastPosition = newPosition;
			POINT cursorPosition;
			GetCursorPos(&cursorPosition);
			Array2i cursor(cursorPosition.x, cursorPosition.y);
			//addRecord(move, newPosition, Array2i(), interval);
			MouseState ms = mouseController->getMouseState();
			addRecord(cursor, ms);
		}, interval));*/

		/*
		mouseMovementRecorder.setFunction([this]() {
			POINT cursorPosition;
			GetCursorPos(&cursorPosition);
			Array2i cursor(cursorPosition.x, cursorPosition.y);
			//addRecord(move, newPosition, Array2i(), interval);
			MouseState ms = mouseController->getMouseState();
			addRecord(cursor, ms);
		});
		mouseMovementRecorder.setRepeat(interval);
		mouseMovementRecorder.start();
		*/

	}
	isRecording = true;
}

void MouseRecorder::update() {
	if(isRecording){
		POINT cursorPosition;
		GetCursorPos(&cursorPosition);
		Array2i cursor(cursorPosition.x, cursorPosition.y);
		//addRecord(move, newPosition, Array2i(), interval);
		//MouseState ms = mouseController->getMouseState();
		addRecord(cursor, currentMouseAction);
		currentMouseAction = MouseAction();
	}
}

void MouseRecorder::end() {
	//if (timerIdentifier){
		//timerManager->killTimer(*timerIdentifier);
		//mouseMovementRecorder.cancel();
		//leftClickDownEvents.end();
		//leftClickUpEvents.end();

	isRecording = false;
	//timerIdentifier = nullptr;
	//}
}

MouseRecord MouseRecorder::getRecord(unsigned int rn) const {
	return recordStream.at(rn);
}

Array2f MouseRecorder::getStartingPosition() const {
	return startingPosition;
}

vector<MouseRecord> MouseRecorder::getRecordStream() const {
	return recordStream;
}

void MouseRecorder::addLeftClickDown() {
	if(isRecording){
		//leftClickDownEvents.addEvent();
		currentMouseAction.setLeftClickDown(true);
	}
}

void MouseRecorder::addLeftClickUp() {
	if (isRecording) {
		//leftClickUpEvents.addEvent();
		currentMouseAction.setLeftClickUp(true);
	}
}

void MouseRecorder::addRightClickDown() {
	if (isRecording) {
		currentMouseAction.setRightClickDown(true);
	}
}

void MouseRecorder::addRightClickUp() {
	if (isRecording) {
		currentMouseAction.setRightClickUp(true);
	}
}

//vector<unsigned int> MouseRecorder::getLeftClicksDown() const {
//	return leftClickDownEvents.getEvents();
//}
//
//vector<unsigned int> MouseRecorder::getLeftClicksUp() const {
//	return leftClickUpEvents.getEvents();
//}

bool MouseRecorder::getIsRecording() const {
	return isRecording;
}

float MouseRecorder::getAverageSpeed() const {
	return distance / totalTime;
}
float MouseRecorder::getDistance() const {
	return distance;
}

MouseRecordPlayer::MouseRecordPlayer(shared_ptr<TimerManager> tm, 
	shared_ptr<MouseAutoInput> mai, vector<MouseRecord> mrs)
: timerManager(tm), mouseAutoInput(mai), recordIndex(0), 
isPlaying(false), isFinished(true)//, recordStream(mrs)
{};

void MouseRecordPlayer::setMouseRecorder(shared_ptr<MouseRecorder> mr) {
	mouseRecorder = mr;
}

bool MouseRecordPlayer::play(unsigned int interval) {
	vector<MouseRecord> mr = mouseRecorder->getRecordStream();
	outputDebugLine(to_string(mr.size()));
	if(isFinished){
		mousePointData.setVector(mr);
	}
	if (mr.size() > 0) {
		outputDebugLine(mr[0].getPosition());
		isPlaying = true;
		isFinished = false;
	}
	return isPlaying;
	//if(mouseRecorder && !mouseRecorder->getIsRecording()){
	//	//unsigned int tId = timerManager->addTimer([](){}, interval);
	//	//recordIndex = 0;
	//	//timerManager->changeTimerFunction(tId, [this, tId]() {
	//	//	
	//	//	
	//	//	if(recordIndex < recordStream.size()){
	//	//		MouseRecord record = recordStream[recordIndex];
	//	//		Array2i pos = record.getPosition();
	//	//		mouseAutoInput->mouseGlobalPosition(pos);

	//	//		/*MouseState state = record.getMouseState();
	//	//		if (state.getLeftClick()) {
	//	//			mouseAutoInput->mouseLeftClick(pos);
	//	//		}*/
	//	//		recordIndex = recordIndex + 1;
	//	//	}
	//	//	else {
	//	//		timerManager->killTimer(tId);
	//	//	}
	//	//});
	//	vector<MouseRecord> mr = mouseRecorder->getRecordStream();
	//	if(mr.size() > 0){
	//		mousePointData.setVector(mouseRecorder->getRecordStream());
	//		mouseMoveTimerEvent.setDelay(0);
	//		mouseMoveTimerEvent.setRepeat(interval);
	//		mouseMoveTimerEvent.setFunction([this]() {
	//			try {
	//				Array2i pos = mousePointData.value().getPosition();
	//				mouseAutoInput->mouseGlobalPosition(pos);
	//			}
	//			catch (out_of_range e) {
	//				outputDebugLine(e.what());
	//				//mouseMoveTimerEvent.cancel();
	//				//cannot cancel inside event!!!! change code here
	//			}
	//			bool canContinue = mousePointData.next();
	//			if (!canContinue) {
	//				bool del = mouseMoveTimerEvent.cancel();
	//				//if (del) {
	//				//	outputDebugLine("del succ");
	//				//}
	//			}
	//		});
	//	}

	//	//QueuedTimer qt;
	//	function<void()> leftClickDownFunc = [this]() {
	//		mouseAutoInput->mouseLeftClickDown();
	//		outputDebugLine("MouseClick");
	//	};
	//	leftClickDownQueue.setFunction(leftClickDownFunc);
	//	leftClickDownQueue.setQueue(mouseRecorder->getLeftClicksDown());

	//	function<void()> leftClickUpFunc = [this]() {
	//		outputDebugLine("SAMETIME");
	//		mouseAutoInput->mouseLeftClickUp();
	//	};

	//	//outputDebugLine(mouseRecorder->getLeftClicksUp());
	//	leftClickUpQueue.setFunction(leftClickUpFunc);
	//	leftClickUpQueue.setQueue(mouseRecorder->getLeftClicksUp());

	//	mouseMoveTimerEvent.start();
	//	//leftClickDownQueue.start();
	//	//leftClickUpQueue.start();
	//	//rightClickDownQueue.start();
	//}
}

bool MouseRecordPlayer::getIsPlaying() const {
	return isPlaying;
}

void MouseRecordPlayer::update() {
	next();
}

bool MouseRecordPlayer::next() {
	if(isPlaying){
		try{
			MouseRecord record = mousePointData.value();
			Array2i pos = record.getPosition();
			//outputDebugLine(pos);
			mouseAutoInput->mouseGlobalPosition(pos);
			MouseAction mouseAction = record.getMouseAction();
			if (mouseAction.getLeftClickDown()) {
				mouseAutoInput->mouseLeftClickDown();
			}
			if (mouseAction.getLeftClickUp()) {
				mouseAutoInput->mouseLeftClickUp();
			}
		}catch (out_of_range e) {
			outputDebugLine(e.what());
		}
		bool hasNext = mousePointData.next();
		if (!hasNext) {
			stop();
		}
		return hasNext;
	}
	return false;
}
void MouseRecordPlayer::pause() {
	if(!isFinished)
	isPlaying = false;
}

void MouseRecordPlayer::stop() {
	isPlaying = false;
	isFinished = true;
	mousePointData.first();
	//mouseMoveTimerEvent.cancel();
	//leftClickDownQueue.cancel();
	//leftClickUpQueue.cancel();
}

InputRecorder::InputRecorder() : isRecording(false), isPaused(false) {};

void InputRecorder::start() {
	if (!isRecording && !isPaused) {
		records.clear();
		outputDebugLine("clear");
	}
	isRecording = true;
}

void InputRecorder::pause() {
	isPaused = true;
	isRecording = false;
}

void InputRecorder::end() {
	isRecording = false;
	isPaused = false;
}

vector<InputRecord> InputRecorder::getRecords() const {
	return records;
}
VectorStream<InputRecord> InputRecorder::getRecordStream() const {
	VectorStream<InputRecord> ir;
	ir.setVector(records);
	return ir;
}

void InputRecorder::addRecord(Array2i pos, MouseAction ma, KeyAction ka) {
	InputRecord ir(pos, ma, ka);
	records.push_back(ir);
}

void InputRecorder::addMouseLeftDown() {
	if(isRecording) currentMouseAction.setLeftClickDown(true);
}

void InputRecorder::addMouseLeftUp() {
	if (isRecording) currentMouseAction.setLeftClickUp(true);
}

void InputRecorder::addMouseRightDown() {
	if (isRecording) currentMouseAction.setRightClickDown(true);
}

void InputRecorder::addMouseRightUp() {
	if (isRecording) currentMouseAction.setRightClickUp(true);
}

void InputRecorder::addKeyDown(unsigned int k) {
	if (isRecording) currentKeyAction.addKeyDown(k);
}

void InputRecorder::addKeyUp(unsigned int k) {
	if (isRecording) currentKeyAction.addKeyUp(k);
}

void InputRecorder::update() {
	if (isRecording) {
		POINT cursorPosition;
		GetCursorPos(&cursorPosition);
		Array2i cursor(cursorPosition.x, cursorPosition.y);

		addRecord(cursor, currentMouseAction, currentKeyAction);
		currentMouseAction = MouseAction();
		currentKeyAction = KeyAction();
	}
}

InputRecordPlayer::InputRecordPlayer(shared_ptr<AutoInput> ai) : isPlaying(false), isFinished(false), autoInput(ai) {};

bool InputRecordPlayer::getIsPlaying() const {
	return isPlaying;
}

void InputRecordPlayer::setRecord(VectorStream<InputRecord> rec) {
	recordStream = rec;
}

bool InputRecordPlayer::play() {
	//if(recordStream){
	if (isFinished) {
		recordStream.first();
	}
	isPlaying = true;
	isFinished = false;
	//}
	return isPlaying;
}

bool InputRecordPlayer::next() {
	if (isPlaying && !isFinished) {
		try {
			InputRecord record = recordStream.value();
			//outputDebugLine(record.getPosition());
			record.play(autoInput);
		}
		catch (out_of_range e) {
			outputDebugLine(e.what());
		}
		bool hasNext = recordStream.next();
		if (!hasNext) {
			stop();
		}
		return hasNext;
	}
	return false;
}

void InputRecordPlayer::pause()
{
	isPlaying = false;
}

void InputRecordPlayer::stop()
{
	isFinished = true;
	pause();
}

void InputRecordPlayer::update() {
	next();
}