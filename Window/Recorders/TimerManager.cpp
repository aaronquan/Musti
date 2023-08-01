#pragma once

#include "TimerManager.h"

TimerManager::TimerManager() : TimerManager(nullptr){};

TimerManager::TimerManager(HWND* h) : hwnd(h), idReference(0) {};

void TimerManager::addHandler(HWND* h) {
	hwnd = h;
}

unsigned int TimerManager::addTimer(function<void()> func, unsigned int mili) {
	SetTimer(*hwnd, idReference, mili, NULL);
	functions.insert(make_pair(idReference, func));
	unsigned int ref = idReference;
	idReference++;
	return ref;
}

void TimerManager::changeTimerFunction(unsigned int timerId, function<void()> func) {
	unordered_map<unsigned int, function<void()>>::iterator find = functions.find(timerId);
	if (find != functions.end()) {
		find->second = func;
	}
}

void TimerManager::runTimers(WPARAM wParam, LPARAM lParam) {
	unordered_map<unsigned int, function<void()>>::const_iterator find = functions.find(wParam);
	if (find != functions.end()) {
		find->second();
	}
}

void TimerManager::killTimer(unsigned int id) {
	
	KillTimer(*hwnd, id);
	functions.erase(id);
}

void __stdcall TimerFunction::func(TimerFunctionInfo* param, bool timerOrWait) {
	if(param){
		if (param->cancel) {
			param->isRunning = false;
			//outputDebugLine("CANCELLED");
			if(param->handle){
				outputDebugLine("CANCELLED");
				try{
					bool hasDeleted = DeleteTimerQueueTimer(nullptr, param->handle, nullptr);
				}
				catch (exception e) {
					outputDebugLine(e.what());
				}
			}
		}
		else if(param->function) (*param->function)();
		if (param->repeat == 0) {
			//param->cancel = true;
		}
	}
}

TimerFunction::TimerFunction(){
	//HANDLE tq = CreateTimerQueue();
	//functionInfo.timerQueue = tq;
};

TimerFunction::~TimerFunction() {
	outputDebugLine("del timer");
	bool b = DeleteTimerQueueEx(functionInfo.timerQueue, nullptr);
}

void TimerFunction::setFunction(function<void()> f) {
	functionInfo.function = make_shared<function<void()>>(f);
}

void TimerFunction::setDelay(unsigned int d) {
	functionInfo.delay = d;
}
void TimerFunction::setRepeat(unsigned int r) {
	functionInfo.repeat = r;
}

bool TimerFunction::start() {
	if (!functionInfo.timerQueue) {
		HANDLE tq = CreateTimerQueue();
		functionInfo.timerQueue = tq;
	}
	if(functionInfo.function){
		functionInfo.cancel = false;
		functionInfo.isRunning = true;
		return CreateTimerQueueTimer(&functionInfo.handle, nullptr, (WAITORTIMERCALLBACK)func, &functionInfo,
		functionInfo.delay, functionInfo.repeat, 0);
	}
	return false;
}

bool TimerFunction::cancel() {
	functionInfo.cancel = true;
	/*if (functionInfo.isRunning && functionInfo.handle) {
		outputDebugLine("Deleting timer");
		functionInfo.isRunning = false;
		//bool b = DeleteTimerQueueTimer(functionInfo.timerQueue, functionInfo.handle, INVALID_HANDLE_VALUE);
	}*/
	return true;
}

QueueTimeEvents::QueueTimeEvents() : isActive(false){};

void QueueTimeEvents::start() {
	lastTime = chrono::steady_clock::now(); 
	clear();
	isActive = true;
}

void QueueTimeEvents::addEvent() {
	if(isActive){
		const chrono::steady_clock::time_point now = chrono::steady_clock::now();
		unsigned int duration = chrono::duration_cast<chrono::milliseconds>(now - lastTime).count();
		events.push_back(duration);
		lastTime = now;
	}
}

void QueueTimeEvents::end() {
	isActive = false;
}

void QueueTimeEvents::clear() {
	events.clear();
}

vector<unsigned int> QueueTimeEvents::getEvents() const {
	return events;
}

QueuedTimer::QueuedTimer() : TimerFunction(), queueFunctionInfo(&functionInfo){};

void __stdcall QueuedTimer::func(QueueTimerFunctionInfo* param, bool timerOrWait) {
	TimerFunction::func(param->functionInfo, timerOrWait);
	param->index++;
	if (param->index < param->queue.size()) {
		param->functionInfo->delay = param->queue[param->index];
		CreateTimerQueueTimer(&param->functionInfo->handle, nullptr, 
		(WAITORTIMERCALLBACK)func,
		param, param->functionInfo->delay, 0, 0);
	}
	else {

	}
}

void QueuedTimer::setQueue(vector<unsigned int> q) {
	queueFunctionInfo.queue = q;
	if(q.size() > 0) functionInfo.delay = q[0];
}
void QueuedTimer::setRepeat(unsigned int r) {
	functionInfo.repeat = 0;
}

bool QueuedTimer::start() {
	if (!functionInfo.timerQueue) {
		HANDLE tq = CreateTimerQueue();
		functionInfo.timerQueue = tq;
	}
	if(queueFunctionInfo.queue.size() > 0){
		queueFunctionInfo.index = 0;
		queueFunctionInfo.functionInfo = &functionInfo;
		return CreateTimerQueueTimer(&functionInfo.handle, nullptr, 
		(WAITORTIMERCALLBACK)func, &queueFunctionInfo,
		functionInfo.delay, 0, 0);
	}
	return false;
}

bool QueuedTimer::cancel() {
	//functionInfo.isRunning = false;
	functionInfo.cancel = true;
	return false;
}