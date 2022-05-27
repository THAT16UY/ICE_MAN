#include "StudentWorld.h"
#include <string>
#include <time.h>
#include <stdlib.h>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::ItemPlacement(int i)
{
	return 0;
}

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
		if (itemX >= x - 2 && itemX <= x + 3 && itemY >= y - 2 && itemY <= y + 3) {
			if (it.at(i)->isVisible()) {
				playSound(SOUND_GOT_GOODIE);
				it.at(i)->setVisible(false);
			}
		}
	}
}
/*
int randomP(int i) {
	srand(time(0));
	i = rand() % 65;
	return i;
}
*/

int ItemPlacement(int number) {
	srand(time(0));
	number = rand() % 65;
	return number;
}