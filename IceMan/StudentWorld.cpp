#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

bool StudentWorld::IsIceThere(int x, int y) {
	if (iceSheet[x][y] != nullptr) {
		if (iceSheet[x][y]->isVisible()) {
			return true;
		}
	}
	return false;
}

void StudentWorld::DestroyIce(int x, int y) {
	if (iceSheet[x][y] != nullptr && iceSheet[x][y]->isVisible()) {
		playSound(SOUND_DIG);
		iceSheet[x][y]->setVisible(false);
	}
	return;
}

void StudentWorld::pickItem(int x, int y, std::vector<Item*> &it) {
	
	for (int i{ 0 }; i < it.size(); i++) {
		int itemX = it.at(i)->getX();
		int itemY = it.at(i)->getY();
		if (itemX >= x && itemX <= x + 4 && itemY >= y && itemY <= y + 4) {
			it.at(i)->setVisible(false);
		}
	}
}