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
			Item* temp{};
			if (id == IID_BOULDER) {
				temp = new Boulder(coordinateX, coordinateY);
				xCoordinatesBoulder.push_back(coordinateX);
				yCoordinatesBoulder.push_back(coordinateY);
				for (int i{ coordinateX }; i < coordinateX + 4; i++) {
					for (int j{ coordinateY }; j < coordinateY + 4; j++) {
						if (iceSheet[i][j] != nullptr && iceSheet[i][j]->isVisible()) {
							iceSheet[i][j]->setVisible(false);
						}
					}
				}
				temp->setVisible(true);
				itemV.push_back(temp);
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
			Item* temp{};
			if (id == IID_BOULDER) {
				temp = new Boulder(coordinateX, coordinateY);
				xCoordinatesBoulder.push_back(coordinateX);
				yCoordinatesBoulder.push_back(coordinateY);
				for (int i{ coordinateX }; i < coordinateX + 4; i++) {
					for (int j{ coordinateY }; j < coordinateY + 4; j++) {
						if (iceSheet[i][j] != nullptr && iceSheet[i][j]->isVisible()) {
							iceSheet[i][j]->setVisible(false);
						}
					}
				}
				temp->setVisible(true);
				itemV.push_back(temp);
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

bool StudentWorld::OffTheGrid(int x, int y) {
	if ((x > 60 || x < 0) || (y > 60 || y < 0)) {
		return true;
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

void StudentWorld::itemInteraction(int x, int y, std::vector<Item*> &it) {
	
	for (unsigned int i{ 0 }; i < it.size(); i++) {
		int itemX = it.at(i)->getX();
		int itemY = it.at(i)->getY();
		if (std::sqrt(pow(x - itemX,2) + pow(y - itemY,2)) < 5.5 && it.at(i)->isGrabbable()) {
			it.at(i)->setVisible(true);
		}
		if (std::sqrt(pow(x - itemX, 2) + pow(y - itemY, 2)) < 4 && it.at(i)->isGrabbable()) {
			if (it.at(i)->isVisible()) {

				if (it.at(i)->getID() == IID_GOLD) {
					playSound(SOUND_GOT_GOODIE);
					iceMan->increaseGold();
				}
				if (it.at(i)->getID() == IID_BARREL) {
					playSound(SOUND_FOUND_OIL);
					*oil_found += 1; 
				}
				if (it.at(i)->getID() == IID_SONAR) {
					playSound(SOUND_GOT_GOODIE);
					iceMan->increaseSonar();
				}
				if (it.at(i)->getID() == IID_WATER_POOL) {
					playSound(SOUND_GOT_GOODIE);
					iceMan->increaseWater();
				}

				it.at(i)->setVisible(false);
				it.at(i)->setGrabbable(false);
			}
		}
	}
}

void StudentWorld::actorInteraction(int x, int y, std::vector<Actor*>& it) {

	for (unsigned int i{ 0 }; i < it.size(); i++) {
		int itemX = it.at(i)->getX();
		int itemY = it.at(i)->getY();
		
		if (it.at(i)->isVisible() && (it.at(i)->getID() == IID_HARD_CORE_PROTESTER || it.at(i)->getID() == IID_PROTESTER) ) {
			if (std::sqrt(pow(x - itemX, 2) + pow(y - itemY, 2)) < 4) {
				iceMan->decreaseHit();
				playSound(SOUND_PLAYER_ANNOYED);
			}
		}

	}
}

void StudentWorld::ShortestPath(int ax, int ay, int x, int y) {
	queue<pair<int, int>> q;
	pair<int, int> goal;

	shortfield[x][y] = new int(0);
	visited[x][y] = true; 
	goal.first = x;
	goal.second = y;

	q.push(goal);

	while (!q.empty()) {

		int currentX = q.front().first;
		int currentY = q.front().second;
		int actorX = ax;
		int actorY = ay;

		cout << "Size of q: " << q.size() << endl;

		q.pop();
		//cout << "checking X: " << currentX << " and Y: " << currentY << endl;
		//*(visited[currentX][currentY]) = true;
		int count = 0;

		if (currentX == actorX && currentY == actorY) {
			cout << "protester found." << endl;
		}
		/*
		if (IsIceThere(currentX, currentY)  || OffTheGrid(currentX, currentY)){
			continue;
		}
		
		if (shortfield[x][y] == shortfield[ax][ax]){
			cout << "Protester found!!" << endl;
			break;
		}

		/*
		if (*(shortfield[currentX][currentY]) != 1000)
		{
			continue;
		}
		*/

		/*
		if (squares.empty()) {
			continue;
		}
		*/

		/*
		delete shortfield[currentX][currentY];
		shortfield[currentX][currentY] = new int(count + 1);
		*/
		/*
		q.push(pair<int, int>(currentX, currentY - 1));
		q.push(pair<int, int>(currentX, currentY + 1));
		q.push(pair<int, int>(currentX - 1, currentY));
		q.push(pair<int, int>(currentX + 1, currentY));
		*/
		
		if (!IsIceThere(currentX, currentY + 1) && !OffTheGrid(currentX, currentY + 1) || visited[currentX][currentY] == false)
		{
			int checkingY = currentY + 1;
			int checkingX = currentX;
			bool M_allowed = true;
			for (int i = currentX; i < currentX + 4; i++) {
				for (int j = currentY; j < currentY + 4; j++) {
					if (IsIceThere(i, j)) {
						M_allowed = false;
						continue;
					}
				}
			}
			if (M_allowed == true) {
				//delete shortfield[currentX, currentY + 1];
				//shortfield[currentX, currentY + 1] = new int(0);
				q.push(pair<int, int>(currentX, currentY + 1));
				visited[currentX][currentY + 1] = true;
			}
		}

		if (!IsIceThere(currentX, currentY - 1) && !OffTheGrid(currentX, currentY - 1) || visited[currentX][currentY] == false) {
			int checkingY = currentY - 1;
			int checkingX = currentX;
			bool M_allowed = true;
			for (int i = currentX; i < currentX + 4; i++) {
				for (int j = currentY; j < currentY + 4; j++) {
					if (IsIceThere(i, j)) {
						M_allowed = false;
						continue;
					}
				}
			}
			if (M_allowed == true) {
				q.push(pair<int, int>(currentX, currentY - 1));
				visited[currentX][currentY - 1] = true;
			}
		}

		if (!IsIceThere(currentX + 1, currentY) && !OffTheGrid(currentX + 1, currentY) || visited[currentX][currentY] == false) {
			int checkingY = currentY;
			int checkingX = currentX + 1;
			bool M_allowed = true;
			for (int i = currentX; i < currentX + 4; i++) {
				for (int j = currentY; j < currentY + 4; j++) {
					if (IsIceThere(i, j)) {
						M_allowed = false;
						continue;
					}
				}
			}
			if (M_allowed == true) {
				q.push(pair<int, int>(currentX + 1, currentY));
				visited[currentX + 1][currentY] = true;
			}
		}

		if (!IsIceThere(currentX - 1, currentY) && !OffTheGrid(currentX - 1, currentY) || visited[currentX][currentY] == false) {
			int checkingY = currentY;
			int checkingX = currentX - 1;
			bool M_allowed = true;
			for (int i = currentX; i < currentX + 4; i++) {
				for (int j = currentY; j < currentY + 4; j++) {
					if (IsIceThere(i, j)) {
						M_allowed = false;
						continue;
					}
				}
			}

			if (M_allowed == true) {
				q.push(pair<int, int>(currentX - 1, currentY));
				visited[currentX - 1][currentY] = true;
			}
		}
		cout << "Size of q: " << q.size() << endl;
		
		//count++;
	}
	
}