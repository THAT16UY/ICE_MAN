#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Algorithm"
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include <chrono>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 64; j++) {
				visited[i][j] = { false };
			}
		}
		
		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 64; j++) {
				shortfield[i][j] = new int(1000);
			}
		}
		
	}

	void setUpItem(int num, const int id);

	virtual int init()
	{
		//For-loop sets up the ice sheet.
		

		for (int xAxis{ 0 }; xAxis < 64; xAxis++) {
			if (xAxis == 30 || xAxis == 31 || xAxis == 32 || xAxis == 33) { continue; }
			for (int yAxis{ 0 }; yAxis < 60; yAxis++) {
				iceSheet[xAxis][yAxis] = new Ice(xAxis, yAxis);
				iceSheet[xAxis][yAxis]->setVisible(true);
			}
		}

		oil_found = new int(0);//At every level we start from 0.
		
		int current_level = getLevel();
		boulder_number = new int(std::min(int(current_level / 2 + 3), 10));
		gold_nugget_number = new int(std::max(5 - current_level / 2, 2));
		oil_barrels_number = new int(std::min(2 + current_level, 21));
	
		setUpItem(*boulder_number, IID_BOULDER); //Bouders are visible TODO: make ice surounding boulder invisible.
		setUpItem(*oil_barrels_number, IID_BARREL); //Oil is not visible
		setUpItem(*gold_nugget_number, IID_GOLD); //Gold is not visible.

		Item* tempSonar{ new Sonar(5,60) };
		tempSonar->setVisible(true);

		Item* tempPool{ new Pool(35,60) };
		tempPool->setVisible(true);

		itemV.push_back(tempSonar);
		itemV.push_back(tempPool);

		iceMan = new Iceman();
		iceMan->setVisible(true);

		protester = new Protester(IID_PROTESTER, 60, 60, GraphObject::left, 1.0, 0);
		protester->setVisible(true);

		HProtester = new HardcoreProtester();
		HProtester->setVisible(true);

		
		actorV.push_back(new Protester(IID_PROTESTER, 30, 30, GraphObject::left, 1.0, 0));
		actorV.push_back(new Protester(IID_PROTESTER, 30, 20, GraphObject::left, 1.0, 0));
		actorV.push_back(new Protester(IID_PROTESTER, 30, 10, GraphObject::left, 1.0, 0));

		std::for_each(actorV.begin(), actorV.end(), [](Actor* &act) {act->setVisible(true); });


		setGameStatText("Lvl: " + std::to_string(getLevel()) +
			" Lives: " + std::to_string(getLives()) +
			" Hlth:" + std::to_string(iceMan->getHit() * 10) +
			" Wtr:" + std::to_string(iceMan->getWater()) +
			" Gld:" + std::to_string(iceMan->getGold()) +
			" Oil_Left:" + std::to_string(*oil_barrels_number) +
			" Sonar:" + std::to_string(iceMan->getSonar()) +
			" Scr:" + std::to_string(getScore()));

		return GWSTATUS_CONTINUE_GAME;
	}

	int ItemPlacement(int i);
	bool IsIceThere(int x, int y);
	bool OffTheGrid(int x, int y);
	void DestroyIce(int x, int y);
	void itemInteraction(int x, int y, std::vector<Item*> &it); //This method handles the collitions with items. TODO - add counts to appropriate fields with in iceman.
	void actorInteraction(int x, int y, std::vector<Actor*> &it);
	//void ShortestPath(int ax, int ay, int x, int y);
	//bool fourbyfourice(int , int , int, int);
	//void ShortestPath(int ax, int ay, int x, int y);
	//bool fourbyfourice(int , int , int, int);
	//void MakingPath(int, int, int, int, std::queue<std::pair<int, int>> &Pdirections);
	//char SmallestDirection(int, int, int, int);
	//void ProtestorMove(int, int);

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		int x = iceMan->getX();
		int y = iceMan->getY();
		
		itemInteraction(x, y, itemV); //Checking for any objects with in a 5.5(to make visible) & 4(to pick up) radius of the iceman.
		actorInteraction(x,y, actorV);

		if (!gunSquirts.empty()) {
			for (auto g : gunSquirts) {
				g->terminate();
			}
		}

		if (*oil_found == *oil_barrels_number) {
			return GWSTATUS_FINISHED_LEVEL;
		}

		int ch;
		if (getKey(ch) == true) {  // user hit a key this tick! 
			bool isBoulder{ false };
			switch (ch)
			{
				
			case KEY_PRESS_LEFT:  // move player to the left ...;
				for (auto rock: itemV) {
					if (rock->getID() != IID_BOULDER || !rock->isVisible()) { continue; }
					if (std::sqrt(pow((x - 1) - rock->getX(), 2) + pow(y - rock->getY(), 2)) < 3) { isBoulder = true;}
				}
				for (unsigned int i{ 0 }; i < actorV.size(); i++) {
					if (std::sqrt(pow((x - 1) - actorV.at(i)->getX(), 2) + pow(y - actorV.at(i)->getY(), 2)) < 3) { isBoulder = true; }
				}

				if (isBoulder) {break;}

				if (x > 0) {
					iceMan->moveTo(x - 1, y);
				}

				iceMan->setDirection(GraphObject::left);
				for(int i = x; i<x+4; i++){
					for (int j = y; j < y + 4; j++) {
						DestroyIce(i, j);
					}
				}
				break;

			case KEY_PRESS_RIGHT:  // move player to the right ...;
				for (auto rock : itemV) {
					if (rock->getID() != IID_BOULDER || !rock->isVisible()) { continue; }
					if (std::sqrt(pow((x + 1) - rock->getX(), 2) + pow(y - rock->getY(), 2)) < 3) { isBoulder = true;}
				}
				
				for (unsigned int i{ 0 }; i < actorV.size(); i++) {
					if (std::sqrt(pow((x + 1) - actorV.at(i)->getX(), 2) + pow(y - actorV.at(i)->getY(), 2)) < 3) { isBoulder = true; }
				}

				if (isBoulder) {break;}

				if (x < 60) {
					iceMan->moveTo(x + 1, y);
				}
	
				iceMan->setDirection(GraphObject::right);
				for (int i = x; i < x + 4; i++) {
					for (int j = y; j < y + 4; j++) {
						DestroyIce(i, j);
					}
				}
				break;

			case KEY_PRESS_DOWN:
				for (auto rock : itemV) {
					if (rock->getID() != IID_BOULDER || !rock->isVisible()) { continue; }
					if (std::sqrt(pow(x - rock->getX(), 2) + pow((y - 1) - rock->getY(), 2)) < 3) { isBoulder = true; }
				}
				for (unsigned int i{ 0 }; i < actorV.size(); i++) {
					if (std::sqrt(pow(x - actorV.at(i)->getX(), 2) + pow((y - 1) - actorV.at(i)->getY(), 2)) < 3) { isBoulder = true; }
				}

				if (isBoulder) {break;}

				if (y > 0) {
					iceMan->moveTo(x, y - 1);
				}
	
				iceMan->setDirection(GraphObject::down);
				for (int i = x; i < x + 4; i++) {
					for (int j = y; j < y + 4; j++) {
						DestroyIce(i, j);
					}
				}
				break;

			case KEY_PRESS_UP:
				for (auto rock : itemV) {
					if (rock->getID() != IID_BOULDER || !rock->isVisible()) { continue; }
					if (std::sqrt(pow(x + rock->getX(), 2) + pow((y + 1) - rock->getY(), 2)) < 3) { isBoulder = true; }
				}
				for (unsigned int i{ 0 }; i < actorV.size(); i++) {
					if (std::sqrt(pow(x - actorV.at(i)->getX(), 2) + pow((y + 1) - actorV.at(i)->getY(), 2)) < 3) { isBoulder = true; }
				}

				if (isBoulder) {break;}

				if (y < 60) {
					iceMan->moveTo(x, y + 1);
				}
		
				iceMan->setDirection(GraphObject::up);
				for(int i = x; i< x + 4; i++){
					for (int j = y; j < y + 4; j++) {
						DestroyIce(i, j);
					}
				}
				break;

			case KEY_PRESS_SPACE:
				if (iceMan->getWater() <= 0) { break; }
				playSound(SOUND_PLAYER_SQUIRT);
				switch (iceMan->getDirection())
				{
				case GraphObject::up:
					gunSquirt = new Gun(iceMan->getX(), iceMan->getY());
					gunSquirt->setDirection(GraphObject::up);
					gunSquirt->setVisible(true);
					gunSquirt->moveTo(x, y + 12);
					break;
				case GraphObject::down:
					gunSquirt = new Gun(iceMan->getX(), iceMan->getY());
					gunSquirt->setDirection(GraphObject::down);
					gunSquirt->setVisible(true);
					gunSquirt->moveTo(x, y - 12);
					break;
				case GraphObject::left:
					gunSquirt = new Gun(iceMan->getX(), iceMan->getY());
					gunSquirt->setDirection(GraphObject::left);
					gunSquirt->setVisible(true);
					gunSquirt->moveTo(x - 12, y);
					break;
				case GraphObject::right:
					gunSquirt = new Gun(iceMan->getX(), iceMan->getY());
					gunSquirt->setDirection(GraphObject::right);
					gunSquirt->setVisible(true);
					gunSquirt->moveTo(x + 12, y);
					break;
				}
				gunSquirts.push_back(gunSquirt);//pointer saved to be deleted later.
				iceMan->decreaseWater();
				break;

			case KEY_PRESS_ESCAPE:
				//TODO: Set this up.
				break;

			case KEY_PRESS_TAB:
				if (iceMan->getGold() > 0) {
					iceMan->decreaseGold();
					Item* tempG{ new Gold(x,y) };
					tempG->setGrabbable(false);
					tempG->setVisible(true);
					itemV.push_back(tempG);
				}
				break;
			case 'z':
			case 'Z':
				if (iceMan->getSonar() == 0) { break; }
				playSound(SOUND_SONAR);
				iceMan->decreaseSonar();
				for (unsigned int i{ 0 }; i < itemV.size(); i++) {
					int itemX = itemV.at(i)->getX();
					int itemY = itemV.at(i)->getY();
					if (std::sqrt(pow(x - itemX, 2) + pow(y - itemY, 2)) < 12 && itemV.at(i)->isGrabbable()) {
						itemV.at(i)->setVisible(true);
					}
				}
				break;
			}
			
		}

		
		for (int i{ 0 }; i < itemV.size(); i++) { //This will check the boulders.
			if (itemV.at(i)->getID() != IID_BOULDER) { continue; }
			
			itemV.at(i)->terminate();

			bool isIce{ false };
			for (int j{ itemV.at(i)->getX() }; j < itemV.at(i)->getX() + 3; j++) {
				if (IsIceThere(j, itemV.at(i)->getY() - 1)) {
					//std::cout << IsIceThere(j, itemV.at(i)->getY() - 1) << std::endl;
					isIce = true;
				}
			}
			if (!isIce) { 
				int a{itemV.at(i)->getY()}; // a represents the y-coordinate
				
				for (int b{ itemV.at(i)->getX() }; b < itemV.at(i)->getX() + 3; b++) {
					for (; a > 0; a--) {
						if (IsIceThere(b,a)) {
							itemV.at(i)->moveTo(itemV.at(i)->getX(), a);
							itemV.at(i)->setFall();
							if (itemV.at(i)->isVisible()) { playSound(SOUND_FALLING_ROCK); }
							break;
						}
					}
				}
				
				
			}
		}
		
		int px = protester->getX();
		int py = protester->getY();
		//ShortestPath(60, 60, 30, 30);
		//if (px != 30) { protester->moveTo(30, 60); } // protester movement
		//if (px == 30) { protester->moveTo(30, 0); }

		setGameStatText("Lvl: " + std::to_string(getLevel()) +
			" Lives: " + std::to_string(getLives()) +
			" Hlth:" + std::to_string(iceMan->getHit() * 10) +
			" Wtr:" + std::to_string(iceMan->getWater()) +
			" Gld:" + std::to_string(iceMan->getGold()) +
			" Oil_Left:" + std::to_string(*oil_barrels_number - *oil_found) +
			" Sonar:" + std::to_string(iceMan->getSonar()) +
			" Scr:" + std::to_string(getScore()));

		//decLives();
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
		delete iceMan;
		delete protester;
		delete HProtester;
		delete boulder_number;
		delete gold_nugget_number;
		delete oil_barrels_number;
		delete oil_found;
		
		for (Item* ite : itemV) {
			delete ite; //Deleting all items in vector.
			ite = nullptr;
		}

		for (Item* item : gunSquirts) {
			delete item;
			item = nullptr;
		}

		gunSquirts.clear();

		itemV.clear();
		
		

		for (int i = 0; i < 64; i++) {//Deleting iceSheet.
			if (i == 31 || i == 32 || i == 33) { continue; }
			for (int j = 0; j < 64; j++) {
				delete iceSheet[i][j];
			}
		}

	}

private:
	
	//The tree variables below will be set to a specific number every level based on the documentation specification.
	int* boulder_number{}; 
	int* gold_nugget_number{};
	int* oil_barrels_number{};

	int* oil_found{}; //Will keep count of how many oil barrels we have found so far.

	Iceman* iceMan{};
	Actor* protester{};//temp, used for testing.
	Protester* HProtester{};//temp, used for testing.

	int* shortfield[64][64]; 
	bool visited[64][64];
	
	Ice* iceSheet[65][65]{ nullptr };
	std::vector<Item*> itemV;//This vector stores items: Boulders,Gold,Oil,Sonar,Pool.
	std::vector<Actor*> actorV;

	std::vector<Gun*> gunSquirts{};
	Gun* gunSquirt{};
};

#endif // STUDENTWORLD_H_