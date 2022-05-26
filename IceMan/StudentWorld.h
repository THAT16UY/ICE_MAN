#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Algorithm"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{

	}

	virtual int init()
	{
		iceMan = new Iceman(); //potential memory leak.
		iceMan->setVisible(true);

		protester = new Protester(IID_PROTESTER, 60, 60, GraphObject::left, 1.0, 0);
		protester->setVisible(true);

		HProtester = new HardcoreProtester();
		HProtester->setVisible(true);

		//FOR TESTING: we will put items in a vector then run a loop to set them visible.
		itemV.push_back(new Gold(10, 60));
		itemV.push_back(new Oil(40, 60));
		itemV.push_back(new Sonar(50, 60));
		itemV.push_back(new Pool(15, 60));

		std::for_each(itemV.begin(), itemV.end(), [](Item* tempItem) { tempItem->setVisible(true); });

		tempBoulder = new Boulder(20,40);// temp display boulder
		tempBoulder->setVisible(true);

		tempGun = new Gun(5,60);
		tempGun->setVisible(true);


		for (int xAxis{ 0 }; xAxis < 64; xAxis++) { // 60 * 60 = 3600 ice objects.......... // 1 = 4 squares  .25 =  square
			if (xAxis == 31 || xAxis == 32 || xAxis == 33) { continue; }
			for (int yAxis{ 0 }; yAxis < 60; yAxis++) {
				iceSheet[xAxis][yAxis] = new Ice(xAxis, yAxis);
				iceSheet[xAxis][yAxis]->setVisible(true);
			}
		}

		

		setGameStatText("Lives: " + std::to_string(getLives()) + " Level: " + std::to_string(getLevel()));

		return GWSTATUS_CONTINUE_GAME;
	}

	bool IsIceThere(int x, int y);
	void DestroyIce(int x, int y);
	void pickItem(int x, int y, std::vector<Item*> &it); //This method handles the collitions with items. TODO - add counts to appropriate fields with in iceman.

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

		int x = iceMan->getX();
		int y = iceMan->getY();

		pickItem(x, y, itemV);

		int ch;
		if (getKey(ch) == true) {  // user hit a key this tick! 

			switch (ch)
			{
			case KEY_PRESS_LEFT:  // move player to the left ...;
				iceMan->moveTo(x - 1, y);
				iceMan->setDirection(GraphObject::left);
				for(int i = x; i<x+4; i++){
					for (int j = y; j < y + 4; j++) {
						DestroyIce(i, j);
					}
				}
				break;
			case KEY_PRESS_RIGHT:  // move player to the right ...;
				iceMan->moveTo(x + 1, y);
				iceMan->setDirection(GraphObject::right);
				for (int i = x; i < x + 4; i++) {
					for (int j = y; j < y + 4; j++) {
						DestroyIce(i, j);
					}
				}
				break;
			case KEY_PRESS_DOWN:
				iceMan->moveTo(x, y - 1);
				iceMan->setDirection(GraphObject::down);
				for (int i = x; i < x + 4; i++) {
					for (int j = y; j < y + 4; j++) {
						DestroyIce(i, j);
					}
				}
				break;
			case KEY_PRESS_UP:
				iceMan->moveTo(x, y + 1);
				iceMan->setDirection(GraphObject::up);
				for(int i = x; i<x+4; i++){
					for (int j = y; j < y + 4; j++) {
						DestroyIce(i, j);
					}
				}
				break;
			case KEY_PRESS_SPACE:  // add a Squirt in front of the player...;

				break;  // etc...  } 
			}
		}
		

		int px = protester->getX();
		int py = protester->getY();
		protester->moveTo(px - 1, py); // protester movement
	

		//decLives();
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
		delete iceMan;
		delete protester;
		delete HProtester;
		delete tempBoulder;
		delete tempGun;
		
		for (Item* ite : itemV) {
			delete ite; //Deleting all items in vector.
		}


		for (int i = 0; i < 64; i++) {//Deleting iceSheet.
			if (i == 31 || i == 32 || i == 33) { continue; }
			for (int j = 0; j < 64; j++) {
				delete iceSheet[i][j];
			}
		}

	}

private:

	Actor* iceMan{};
	Actor* protester{};
	Protester* HProtester{};
	Item* tempBoulder{};
	Item* tempGun{};
	
	

	Ice* iceSheet[65][65]{ nullptr };
	std::vector<Item*> itemV;
};

#endif // STUDENTWORLD_H_