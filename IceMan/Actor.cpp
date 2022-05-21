#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) :
	GraphObject(imageID, startX, startY, dir, size, depth) { }
Actor::~Actor() { }

//Iceman will take no arguments as it will always appear in the same location, with all the same arguments.
Iceman::Iceman() : Actor(IID_PLAYER, 30, 60, right, 1.0, 0) { }

void Iceman::increaseWater() {
	water_units++;
}
void Iceman::increaseSonar() {
	sonar++;
}
void Iceman::increaseGold() {
	gold++;
}
void Iceman::decreaseWater() {
	water_units--;
}
void Iceman::decreaseSonar() {
	sonar--;
}
void Iceman::decreaseGold() {
	gold--;
}
void Iceman::decreaseHit() { //TODO: Have this method return continue or end game based on the hit_points left.
	hit_points--;
}

void Iceman::doSomething() {

	//int ch;
	//if (getWorld()->getKey(ch) == true) {  // user hit a key this tick! 
	//	switch (ch)
	//	{
	//	case KEY_PRESS_LEFT:  // move player to the left ...;
	//		break;
	//	case KEY_PRESS_RIGHT:  // move player to the right ...;
	//		break;
	//	case KEY_PRESS_SPACE:  // add a Squirt in front of the player...;
	//		break;  // etc...  } 
	//	}
	//}
}

Iceman::~Iceman() { }

//-----------------------------------------------------------------------------------------------------------------------PROTESTER START

Protester::Protester(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) :
	Actor(imageID, startX, startY, dir, size, depth) { }

Protester::~Protester(){}

//----------------------------------------------------------------------------------------------------------------------PROTESTER END

//----------------------------------------------------------------------------------------------------------------------H PROTESTER START
HardcoreProtester::HardcoreProtester() :
	Protester(IID_HARD_CORE_PROTESTER, 55, 60, left, 1.0, 0){ }

HardcoreProtester::~HardcoreProtester(){}
//----------------------------------------------------------------------------------------------------------------------H PROTESTER END

//-----------------------------------------------------------------------------------------------------------------------ITEM START
Item::Item(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) :
	GraphObject(imageID, startX, startY, dir, size, depth) {}

Item::~Item() {}
//-----------------------------------------------------------------------------------------------------------------------ITEM END

//-----------------------------------------------------------------------------------------------------------------------ICE START
Ice::Ice(int startX, int startY) : 
	Item(IID_ICE, startX, startY, none, 0.25, 1) {}

Ice::~Ice() {}
//-----------------------------------------------------------------------------------------------------------------------ICE END
 
//-----------------------------------------------------------------------------------------------------------------------GOLD START
Gold::Gold(int startX, int startY) :
	Item(IID_GOLD , startX , startY , none , 1, 0) {}

//TODO : in the init() function, within the IceSheet cordinates declare gold xAxis & yAxis using random numbers.

Gold::~Gold() {}
//-----------------------------------------------------------------------------------------------------------------------GOLD END

//-----------------------------------------------------------------------------------------------------------------------OIL START
Oil::Oil(int startX, int startY) : Item(IID_BARREL, startX, startY, none, 1.0, 0){}

//TODO : in the init() function, within the IceSheet cordinates declare oil xAxis & yAxis using random numbers.

Oil::~Oil() {}
//-----------------------------------------------------------------------------------------------------------------------OIL END

//-----------------------------------------------------------------------------------------------------------------------BOULDER START 
Boulder::Boulder(int startX, int startY) : Item(IID_BOULDER, startX, startY, none, 1.0, 10){}

//TODO : in the init() function, within the IceSheet cordinates declare Boulder xAxis & yAxis using random numbers.
//       also add the dropping functionality.

Boulder::~Boulder(){}
//-----------------------------------------------------------------------------------------------------------------------BOULDER END

//-----------------------------------------------------------------------------------------------------------------------SONAR START
Sonar::Sonar(int startX, int startY) : Item(IID_SONAR, startX, startY, none, 1.0, 0){}

//TODO : in the init() function, within the IceSheet cordinates declare Sonar xAxis & yAxis using random numbers.

Sonar::~Sonar(){}
//-----------------------------------------------------------------------------------------------------------------------SONAR END

//-----------------------------------------------------------------------------------------------------------------------GUN START
Gun::Gun(int startX, int startY) : Item(IID_WATER_SPURT, startX, startY, none, 1.0, 0) {}

//TODO : in the init() function, within the IceSheet cordinates declare Gun xAxis & yAxis using random numbers.

Gun::~Gun() {}
//-----------------------------------------------------------------------------------------------------------------------GUN END

//-----------------------------------------------------------------------------------------------------------------------POOL START
Pool::Pool(int startX, int startY) : Item(IID_WATER_POOL, startX, startY, none, 1.0, 0) {}

//TODO : in the init() function, within the IceSheet cordinates declare Pool xAxis & yAxis using random numbers.

Pool::~Pool() {}
//-----------------------------------------------------------------------------------------------------------------------POOL END

