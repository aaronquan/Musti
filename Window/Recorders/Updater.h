#pragma once

#include <vector>
#include <memory>
#include <functional>

using namespace std;

//change specific things inside a class periodically
class Updater {
public:
	virtual void update() = 0;
};

class UpdaterCollection : public Updater {
private:
	vector<shared_ptr<Updater>> updaters;
	vector<function<void()>> funcs;
public:
	UpdaterCollection();
	void addUpdater(shared_ptr<Updater> upd);
	void addFunction(function<void()> f);
	void update();
};