#pragma once

#include "Updater.h"

UpdaterCollection::UpdaterCollection(){};

void UpdaterCollection::addUpdater(shared_ptr<Updater> upd) {
	updaters.push_back(upd);
}

void UpdaterCollection::addFunction(function<void()> f) {
	funcs.push_back(f);
}

void UpdaterCollection::update() {
	for (shared_ptr<Updater> upd: updaters) {
		upd->update();
	}
	for (function<void()> func : funcs) {
		func();
	}
}