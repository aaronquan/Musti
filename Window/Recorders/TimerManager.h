#pragma once

#include <windows.h>

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <vector>
#include <chrono>

#include "../Util.h"

using namespace std;

class SingleTimer {
private:
	chrono::steady_clock::time_point lastTime;
	bool isRunning;
public:
	SingleTimer();

};

class TimerManager {
private:
	// map of timer identifiers to their functions
	unordered_map<unsigned int, function<void()>> functions; 

	HWND* hwnd;

	unsigned int idReference;


public:
	TimerManager();
	TimerManager(HWND* h);
	void addHandler(HWND* h);
	unsigned int addTimer(function<void()> func, unsigned int mili);
	void changeTimerFunction(unsigned int timerId, function<void()> func);
	void runTimers(WPARAM wParam, LPARAM lParam);
	void killTimer(unsigned int id);
};



class TimerFunction {
protected:
	struct TimerFunctionInfo {
		shared_ptr<function<void()>> function;
		HANDLE timerQueue;
		HANDLE handle;
		unsigned int delay;
		unsigned int repeat;
		bool cancel;
		bool isRunning;
		TimerFunctionInfo() : function(nullptr), timerQueue(nullptr), handle(nullptr), delay(0), 
		repeat(0), isRunning(false), cancel(false) {};
	};
	static void __stdcall func(TimerFunctionInfo* param, bool timerOrWait);
	//HANDLE timerQueue;

	TimerFunctionInfo functionInfo;
public:
	TimerFunction();
	~TimerFunction();
	void setFunction(function<void()> f);
	void setDelay(unsigned int d);
	virtual void setRepeat(unsigned int r);
	virtual bool start();
	//void pause();
	bool cancel();
};

class ControlTimerFunction {
private:
	chrono::steady_clock::time_point lastTime;
public:

	void pause();
};

class QueueTimes {
private:
	vector<unsigned int> events;
public:
	QueueTimes();

};

class QueuedTimer : public TimerFunction {
private:
	struct QueueTimerFunctionInfo {
		TimerFunctionInfo* functionInfo;
		vector<unsigned int> queue;
		unsigned int index;
		QueueTimerFunctionInfo(TimerFunctionInfo* fi) : functionInfo(fi), index(0){};
	};
	QueueTimerFunctionInfo queueFunctionInfo;

	static void __stdcall func(QueueTimerFunctionInfo* param, bool timerOrWait);
public:
	QueuedTimer();
	void setRepeat(unsigned int r);
	void setQueue(vector<unsigned int> q);
	bool start();
	bool cancel();
};

class QueueTimeEvents {
private:
	vector<unsigned int> events;
	chrono::steady_clock::time_point lastTime;
	bool isActive;
public:
	QueueTimeEvents();
	void start();
	void addEvent();
	void end();
	void clear();
	vector<unsigned int> getEvents() const;
};