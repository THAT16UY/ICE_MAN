#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
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

		tempGold = new Gold(10,60); //temp displays gold
		tempGold->setVisible(true);
		
		tempOil = new Oil(40,60); //temp displays Oil
		tempOil->setVisible(true);

		tempBoulder = new Boulder(20,40);// temp display boulder
		tempBoulder->setVisible(true);

		tempPool = new Pool(15,60);
		tempPool->setVisible(true);

		tempGun = new Gun(5,60);
		tempGun->setVisible(true);

		tempSonar = new Sonar(50,60);
		tempSonar->setVisible(true);


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
	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

		int x = iceMan->getX();
		int y = iceMan->getY();

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
		


		//if (iceSheet[x + 5][y] != nullptr) { //TODO: Fix - Throws access violation exception.
		//	iceSheet[x + 4][y]->setVisible(false);
		//	iceSheet[x + 4][y + 1]->setVisible(false);
		//	iceSheet[x + 4][y + 2]->setVisible(false);
		//	iceSheet[x + 4][y + 3]->setVisible(false);
		//}

		//iceMan->moveTo(x + 1, y); //Iceman movement.

		int px = protester->getX();
		int py = protester->getY();
		protester->moveTo(px - 1, py); // protester movement
		

		//decLives();
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
		delete iceMan;
		delete tempGold;
		delete tempOil;
	}

private:

	Actor* iceMan{}; //TODO: Fix memory leak
	Actor* protester{};
	Protester* HProtester{};
	Item* tempOil{};
	Item* tempGold{};//For testing purposes only.
	Item* tempBoulder{};

	Item* tempGun{};
	Item* tempPool{};
	Item* tempSonar{};

	Ice* iceSheet[65][65]{ nullptr }; //TODO: Fix memory leak - will delete in the cleanUp() function.
};

#endif // STUDENTWORLD_H_