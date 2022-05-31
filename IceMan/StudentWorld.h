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

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
		
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

		iceMan = new Iceman();
		iceMan->setVisible(true);

		protester = new Protester(IID_PROTESTER, 60, 60, GraphObject::left, 1.0, 0);
		protester->setVisible(true);

		HProtester = new HardcoreProtester();
		HProtester->setVisible(true);

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
	void DestroyIce(int x, int y);
	void itemInteraction(int x, int y, std::vector<Item*> &it); //This method handles the collitions with items. TODO - add counts to appropriate fields with in iceman.
	void ProtesterMovement(int x, int y);

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		int x = iceMan->getX();
		int y = iceMan->getY();
		
		itemInteraction(x, y, itemV); //Checking for any objects with in a 5.5(to make visible) & 4(to pick up) radius of the iceman.

		if (*oil_found == *oil_barrels_number) {
			return GWSTATUS_FINISHED_LEVEL;
		}

		int ch;
		if (getKey(ch) == true) {  // user hit a key this tick! 

			switch (ch)
			{
			case KEY_PRESS_LEFT:  // move player to the left ...;
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
				//TODO: Find solution to making the splash invisible after it has moved, and add coalition detection with protester.
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
			}
			
		}
		
		int px = protester->getX();
		int py = protester->getY();
		//protester->moveTo(px - 1, py); // protester movement
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
	
	Ice* iceSheet[65][65]{ nullptr };
	std::vector<Item*> itemV;//This vector stores items: Boulders,Gold,Oil,Sonar,Pool.

};

#endif // STUDENTWORLD_H_