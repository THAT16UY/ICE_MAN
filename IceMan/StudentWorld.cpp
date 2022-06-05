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
	if ((x > 64 || x < 0) || (y > 64 || y < 0)) {
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
	bool actorfound = false;

	q.push(goal);

	while (actorfound == false) {

		int currentX = q.front().first;
		int currentY = q.front().second;
		int actorX = ax;
		int actorY = ay;

		cout << "Size of q: " << q.size() << endl;
		cout << "Testing " << currentX << " " << currentY << endl;
		cout << currentX << " , " << currentY << " value: " << *(shortfield[currentX][currentY]) << endl;
		q.pop();
		//cout << "checking X: " << currentX << " and Y: " << currentY << endl;
		//*(visited[currentX][currentY]) = true;
		int count = 0;

		if (currentX == actorX && currentY == actorY) {
			cout << "protester found." << endl;
			actorfound = true;
		}
		else {
			if (!OffTheGrid(currentX, currentY + 1) && visited[currentX][currentY + 1] == false)
			{
				int checkingY = currentY + 1;
				int checkingX = currentX;
				bool M_allowed = true;
				if (!fourbyfourice(checkingX, checkingY, checkingX + 4, checkingY + 4)) { M_allowed = false; }
				if (M_allowed == true) {
					q.push(pair<int, int>(currentX, currentY + 1));
					cout << "up pass " << endl;
					visited[currentX][currentY + 1] = true;
					delete shortfield[currentX][currentY + 1];
					shortfield[currentX][currentY + 1] = new int(*(shortfield[currentX][currentY]) + 1);
					cout << checkingX << " , " << checkingY << " value: " << *(shortfield[currentX][currentY + 1]) << endl;
				}
				else {
					cout << "up fail " << endl;
				}
			}

			if (!OffTheGrid(currentX, currentY - 1) && visited[currentX][currentY - 1] == false) {
				int checkingY = currentY - 1;
				int checkingX = currentX;
				bool M_allowed = true;
				if (!fourbyfourice(checkingX, checkingY, checkingX + 4, checkingY + 4)) { M_allowed = false; }
				if (M_allowed == true) {
					q.push(pair<int, int>(currentX, currentY - 1));
					cout << "down pass " << endl;
					visited[currentX][currentY - 1] = true;
					delete shortfield[currentX][currentY - 1];
					shortfield[currentX][currentY - 1] = new int(*(shortfield[currentX][currentY]) + 1);
					cout << checkingX << " , " << checkingY << " value: " << *(shortfield[currentX][currentY - 1]) << endl;
				}
				else {
					cout << "down fail " << endl;
				}
			}

			if (!OffTheGrid(currentX + 1, currentY) && visited[currentX + 1][currentY] == false) {
				int checkingY = currentY;
				int checkingX = currentX + 1;
				bool M_allowed = true;
				if (!fourbyfourice(checkingX, checkingY, checkingX + 4, checkingY + 4)) { M_allowed = false; }
				if (M_allowed == true) {
					q.push(pair<int, int>(currentX + 1, currentY));
					cout << "right pass " << endl;
					visited[currentX + 1][currentY] = true;
					delete shortfield[currentX + 1][currentY];
					shortfield[currentX + 1][currentY] = new int(*(shortfield[currentX][currentY]) + 1);
					cout << checkingX << " , " << checkingY << " value: " << *(shortfield[currentX + 1][currentY]) << endl;
				}
				else {
					cout << "right fail " << endl;
				}
			}

			if (!OffTheGrid(currentX - 1, currentY) && visited[currentX - 1][currentY] == false) {
				int checkingY = currentY;
				int checkingX = currentX - 1;
				bool M_allowed = true;
				if (!fourbyfourice(checkingX, checkingY, checkingX + 4, checkingY + 4)) { M_allowed = false; }
				if (M_allowed == true) {
					q.push(pair<int, int>(currentX - 1, currentY));
					cout << "left pass " << endl;
					visited[currentX - 1][currentY] = true;
					delete shortfield[currentX - 1][currentY];
					shortfield[currentX - 1][currentY] = new int(*(shortfield[currentX][currentY]) + 1);
					cout << checkingX << " , " << checkingY << " value: " << *(shortfield[currentX - 1][currentY]) << endl;
				}
				else {
					cout << "left fail " << endl;
				}

			}

			cout << "Size of q: " << q.size() << endl;
		}
	}
	
}

bool StudentWorld::fourbyfourice(int x, int y, int extraX, int extraY) {
	for (x; x < extraX; x++) {
		for (y; y < extraY; y++){
			if (OffTheGrid(x,y)) {
				if (iceSheet[x][y]->isVisible()) {
					return false;
				}
			}
			return true;
		}
		
	}
	
}

char StudentWorld::SmallestDirection(int UP, int DOWN, int RIGHT, int LEFT) {
	char SmallestY;
	int Yvalue;
	char SmallestX;
	int Xvalue;
	char SmallestDir;
	if (UP < DOWN) {
		SmallestY = 'U';
		Yvalue = UP;
	}
	else {
		SmallestY = 'D';
		Yvalue = DOWN;
	}
	if (LEFT < RIGHT) {
		SmallestX = 'L';
		Xvalue = LEFT;
	}
	else {
		SmallestX = 'R';
		Xvalue = RIGHT;
	}
	if (Yvalue > Xvalue) {
		SmallestDir = SmallestX;
	}
	else {
		SmallestDir = SmallestY;
	}
	return SmallestDir;
}

void StudentWorld::MakingPath(int ax, int ay, int x, int y, std::queue<std::pair<int,int>> &Pdirections) {
	queue<pair<int, int>> Qgrab;
	pair<int, int> Qdirect;

	Qdirect.first = ax;
	Qdirect.second = ay;

	Qgrab.push(Qdirect);

	int Up = 0;
	int Down = 0;
	int Left = 0;
	int Right = 0;

	bool GoalFound = false;

	while (GoalFound == false) {
		int currentX = Qgrab.front().first;
		int currentY = Qgrab.front().second;

		Qgrab.pop();

		if ((currentX == x) && (currentY == y)) {
			cout << " goal found " << endl;
			GoalFound = true;
		}
		else {
			if (!OffTheGrid(currentX, currentY + 1) && IsIceThere(currentX, currentY + 1))
			{
				Up = *(shortfield[currentX][currentY + 1]);
			}
			if (!OffTheGrid(currentX, currentY - 1) && IsIceThere(currentX, currentY - 1))
			{
				Down = *(shortfield[currentX][currentY - 1]);
			}
			if (!OffTheGrid(currentX + 1, currentY) && IsIceThere(currentX + 1, currentY))
			{
				Right = *(shortfield[currentX + 1][currentY]);
			}
			if (!OffTheGrid(currentX - 1, currentY) && IsIceThere(currentX - 1, currentY))
			{
				Left = *(shortfield[currentX - 1][currentY]);
			}

			switch (SmallestDirection(Up, Down, Right, Left)) {
			case 'U':
				Pdirections.push(pair<int, int>(currentX, currentY + 1));
				Qgrab.push(pair<int, int>(currentX, currentY + 1));
			case 'D':
				Pdirections.push(pair<int, int>(currentX, currentY - 1));
				Qgrab.push(pair<int, int>(currentX, currentY - 1));
			case 'L':
				Pdirections.push(pair<int, int>(currentX + 1, currentY));
				Qgrab.push(pair<int, int>(currentX + 1, currentY));
			case 'R':
				Pdirections.push(pair<int, int>(currentX - 1, currentY));
				Qgrab.push(pair<int, int>(currentX - 1, currentY));
			}
		}



	}
}

void StudentWorld::ProtestorMove(int ax, int ay) {
	srand(time(NULL));

	int coordinateX = rand() % 60;
	int coordinateY = rand() % 60;

	if (!IsIceThere(coordinateX, coordinateY) && std::sqrt(pow(ax - coordinateX, 2) + pow(ay - coordinateY, 2)) < 30) {
		ShortestPath(ax, ay, coordinateX, coordinateY);
		queue<pair<int, int>> direct;
		pair<int, int> ProtesterStart;
		MakingPath(ax, ay, coordinateX, coordinateY, direct);
	}
}