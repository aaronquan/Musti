#pragma once

#include "Updater.h"

UpdaterCollection::UpdaterCollection(){};

void UpdaterCollection::addUpdater(shared_ptr<Updater> upd) {
	updaters.push_back(upd);
}

void UpdaterCollection::update() {
	for (shared_ptr<Updater> upd: updaters) {
		upd->update();
	}
}