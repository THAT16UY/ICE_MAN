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
		
		int current_level = getLevel();
		boulder_number = std::min(int(current_level / 2 + 3), 10);
		gold_nugget_number = std::max(5 - current_level / 2, 2);//TODO: integrate into loop.
		oil_barrels_number = std::min(2 + current_level, 21);//Formula => min(2 + current_level_number, 21)
	
		setUpItem(boulder_number, IID_BOULDER); //Bouders are visible
		setUpItem(oil_barrels_number, IID_BARREL); //Oil is not visible
		setUpItem(gold_nugget_number, IID_GOLD); //Gold is not visible.


		iceMan = new Iceman(); //potential memory leak.
		iceMan->setVisible(true);

		protester = new Protester(IID_PROTESTER, 60, 60, GraphObject::left, 1.0, 0);
		//protester->setVisible(true);

		HProtester = new HardcoreProtester();
		//HProtester->setVisible(true);

		for (int xAxis{ 0 }; xAxis < 64; xAxis++) { // 60 * 60 = 3600 ice objects.......... // 1 = 4 squares  .25 =  square
			if (xAxis == 30 || xAxis == 31 || xAxis == 32 || xAxis == 33) { continue; }
			for (int yAxis{ 0 }; yAxis < 60; yAxis++) {
				iceSheet[xAxis][yAxis] = new Ice(xAxis, yAxis);
				iceSheet[xAxis][yAxis]->setVisible(true);
			}
		}

		

		setGameStatText("Lvl: " + std::to_string(getLevel()) +
			" Lives: " + std::to_string(getLives()) +
			" Hlth:" + std::to_string(iceMan->getHit() * 10) +
			" Wtr:" + std::to_string(iceMan->getWater()) +
			" Gld:" + std::to_string(iceMan->getGold()) +
			" Oil_Left:" + std::to_string(oil_barrels_number) +
			" Sonar:" + std::to_string(iceMan->getSonar()) +
			" Scr:" + std::to_string(getScore()));

		return GWSTATUS_CONTINUE_GAME;
	}

	int ItemPlacement(int i);
	bool IsIceThere(int x, int y);
	void DestroyIce(int x, int y);
	void itemInteraction(int x, int y, std::vector<Item*> &it); //This method handles the collitions with items. TODO - add counts to appropriate fields with in iceman.

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

		int x = iceMan->getX();
		int y = iceMan->getY();
		

		itemInteraction(x, y, itemV);

		if (oil_found == oil_barrels_number) {
			return GWSTATUS_PLAYER_DIED;
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
				
				GraphObject::Direction tempDirection = iceMan->getDirection();
				tempGuns.push_back(new Gun(x, y));
				
				switch (tempDirection)
				{
				case GraphObject::right:
					tempGuns.at(tempGuns.size() - 1)->setDirection(GraphObject::right);
					tempGuns.at(tempGuns.size() - 1)->setVisible(true);
					tempGuns.at(tempGuns.size() - 1)->moveTo(x + 5, y);
					break;
				case GraphObject::left:
					tempGuns.at(tempGuns.size() - 1)->setDirection(GraphObject::left);
					tempGuns.at(tempGuns.size() - 1)->setVisible(true);
					tempGuns.at(tempGuns.size() - 1)->moveTo(x - 5, y);
					break;
				case GraphObject::up:
					tempGuns.at(tempGuns.size() - 1)->setDirection(GraphObject::up);
					tempGuns.at(tempGuns.size() - 1)->setVisible(true);
					tempGuns.at(tempGuns.size() - 1)->moveTo(x, y + 5);
					break;
				case GraphObject::down:
					tempGuns.at(tempGuns.size() - 1)->setDirection(GraphObject::down);
					tempGuns.at(tempGuns.size() - 1)->setVisible(true);
					tempGuns.at(tempGuns.size() - 1)->moveTo(x, y - 5);
					break;
				}
				break;  // etc...  } 
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
			" Oil_Left:" + std::to_string(oil_barrels_number - oil_found) +
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
		delete tempBoulder;
		
		
		//for (Item* ite : itemV) {
		//	delete ite; //Deleting all items in vector.
		//}

		//for (Item* ite : tempGuns) {
		//	delete ite;
		//}


		for (int i = 0; i < 64; i++) {//Deleting iceSheet.
			if (i == 31 || i == 32 || i == 33) { continue; }
			for (int j = 0; j < 64; j++) {
				delete iceSheet[i][j];
			}
		}

	}

private:
	
	//The tree variables below will be set to a specific number every level based on the documentation specification.
	int boulder_number{}; 
	int gold_nugget_number{};
	int oil_barrels_number{};

	int oil_found{ 0 }; //Will keep count of how many oil barrels we have found so far.


	Iceman* iceMan{};
	Actor* protester{};
	Protester* HProtester{};
	Item* tempBoulder{};
	
	
	

	Ice* iceSheet[65][65]{ nullptr };
	std::vector<Item*> itemV;
	std::vector<Item*> tempGuns; //No sure if this is the best approach, make sure to review code after.
};

#endif // STUDENTWORLD_H_