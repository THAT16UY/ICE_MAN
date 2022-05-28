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
void StudentWorld::setUpItem(int num, const int id) {
	srand(time(NULL)); //Random number generator. 
	int coordinateX{}, coordinateY{};
	int boulder_count{ 0 };
	bool boulderOk = false, create{};

	while (!boulderOk) {
		coordinateX = rand() % 60;
		coordinateY = rand() % 45 + 10;
		create = true;

		if (coordinateX >= 30 && coordinateX <= 33) {
			coordinateX += 4;
		}
		else if (coordinateX >= 27 && coordinateX <= 29) {
			coordinateX -= 4;
		}

		if (itemV.empty()) {
			if (id == IID_BOULDER) {
				itemV.push_back(new Boulder(coordinateX, coordinateY));
			}
			if (id == IID_GOLD) {
				itemV.push_back(new Gold(coordinateX, coordinateY));
			}
			if (id == IID_BARREL) {
				itemV.push_back(new Oil(coordinateX, coordinateY));
			}
			
			boulder_count++;
		}
		else {
			for (unsigned int i{ 0 }; i < itemV.size(); i++) {
				//if statement uses distance formula to make sure we don't overlap our boulders.
				if (std::sqrt(pow(itemV.at(i)->getX() - coordinateX, 2) + pow(itemV.at(i)->getY() - coordinateY, 2)) < 6) {
					create = false;
					break;
				}
			}
		}

		if (!create) {
			continue;
		}
		else {
			if (id == IID_BOULDER) {
				itemV.push_back(new Boulder(coordinateX, coordinateY));
			}
			if (id == IID_GOLD) {
				itemV.push_back(new Gold(coordinateX, coordinateY));
			}
			if (id == IID_BARREL) {
				itemV.push_back(new Oil(coordinateX, coordinateY));
			}
			boulder_count++;
		}

		if (boulder_count == num) {
			boulderOk = true;
		}

	}
}

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
	
	for (unsigned int i{ 0 }; i < it.size(); i++) {
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

//int ItemPlacement(int number) {
//	srand(time(0));
//	number = rand() % 65;
//	return number;
//}