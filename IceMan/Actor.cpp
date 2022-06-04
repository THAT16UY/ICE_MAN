#include "Actor.h"



// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, StudentWorld* world, Direction dir, double size, unsigned int depth) :
	GraphObject(imageID, startX, startY, dir, size, depth) {}
Actor::~Actor() { }

//Iceman will take no arguments as it will always appear in the same location, with all the same arguments.
Iceman::Iceman() : Actor(IID_PLAYER, 30, 60, nullptr, right, 1.0, 0) { }

void Iceman::increaseWater() {
	water_units +=5;
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

int Iceman::getHit() {
	return hit_points;
}

int Iceman::getWater() {
	return water_units;
}

int Iceman::getSonar() {
	return sonar;
}

int Iceman::getGold() {
	return gold;
}

void Iceman::doSomething() {

}

Iceman::~Iceman() { }

//-----------------------------------------------------------------------------------------------------------------------PROTESTER START

Protester::Protester(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) :
	Actor(imageID, startX, startY, nullptr, dir, size, depth) { }

void Protester::doSomething() {

}

void Protester::PMovement(int ax, int ay, int x, int y) {
	

}

Protester::~Protester() {}

//----------------------------------------------------------------------------------------------------------------------PROTESTER END

//----------------------------------------------------------------------------------------------------------------------H PROTESTER START
HardcoreProtester::HardcoreProtester() :
	Protester(IID_HARD_CORE_PROTESTER, 55, 60, left, 1.0, 0) { }

HardcoreProtester::~HardcoreProtester() {}
//----------------------------------------------------------------------------------------------------------------------H PROTESTER END

//-----------------------------------------------------------------------------------------------------------------------ITEM START
Item::Item(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) :
	GraphObject(imageID, startX, startY, dir, size, depth) {}

Item::~Item() {}
//-----------------------------------------------------------------------------------------------------------------------ITEM END

//-----------------------------------------------------------------------------------------------------------------------ICE START
Ice::Ice(int startX, int startY) :
	Item(IID_ICE, startX, startY, none, 0.25, 1) {}

bool Ice::isGrabbable() {
	return false;
}
void Ice::setGrabbable(bool val) {
	return; //nothing needed here
}

Ice::~Ice() {}
//-----------------------------------------------------------------------------------------------------------------------ICE END

//-----------------------------------------------------------------------------------------------------------------------GOLD START
Gold::Gold(int startX, int startY) :
	Item(IID_GOLD, startX, startY, none, 1, 0) {}

bool Gold::isGrabbable() {
	return is_available;
}

void Gold::setGrabbable(bool val) {
	is_available = val;
}

Gold::~Gold() {}
//-----------------------------------------------------------------------------------------------------------------------GOLD END

//-----------------------------------------------------------------------------------------------------------------------OIL START
Oil::Oil(int startX, int startY) : Item(IID_BARREL, startX, startY, none, 1.0, 0) {}

//TODO : in the init() function, within the IceSheet cordinates declare oil xAxis & yAxis using random numbers.
bool Oil::isGrabbable() {
	return is_available;
}

void Oil::setGrabbable(bool val) {
	is_available = val;
}

Oil::~Oil() {}
//-----------------------------------------------------------------------------------------------------------------------OIL END

//-----------------------------------------------------------------------------------------------------------------------BOULDER START 
Boulder::Boulder(int startX, int startY) : Item(IID_BOULDER, startX, startY, none, 1.0, 10) {}


bool Boulder::isGrabbable() {
	return false;
}

void Boulder::setGrabbable(bool val) {
	return;
}

Boulder::~Boulder() {}
//-----------------------------------------------------------------------------------------------------------------------BOULDER END

//-----------------------------------------------------------------------------------------------------------------------SONAR START
Sonar::Sonar(int startX, int startY) : Item(IID_SONAR, startX, startY, none, 1.0, 0) {}

bool Sonar::isGrabbable() {
	return is_available;
}
void Sonar::setGrabbable(bool val) {
	is_available = val;
}

Sonar::~Sonar() {}
//-----------------------------------------------------------------------------------------------------------------------SONAR END

//-----------------------------------------------------------------------------------------------------------------------GUN START
Gun::Gun(int startX, int startY) : Item(IID_WATER_SPURT, startX, startY, none, 1.0, 0) {
	start = std::chrono::steady_clock::now();
}

//TODO : in the init() function, within the IceSheet cordinates declare Gun xAxis & yAxis using random numbers.
bool Gun::isGrabbable() {
	return false;
}
void Gun::setGrabbable(bool val) {
	return;
}
void Gun::terminate() {

	if (!this->isVisible()) { return; }

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;

	if (elapsed_seconds.count() > .5) {
		this->setVisible(false);
	}

}
Gun::~Gun() {}
//-----------------------------------------------------------------------------------------------------------------------GUN END

//-----------------------------------------------------------------------------------------------------------------------POOL START
Pool::Pool(int startX, int startY) : Item(IID_WATER_POOL, startX, startY, none, 1.0, 0) {}

//TODO : in the init() function, within the IceSheet cordinates declare Pool xAxis & yAxis using random numbers.
bool Pool::isGrabbable() {
	return is_available;
}

void Pool::setGrabbable(bool val) {
	is_available = val;
}

Pool::~Pool() {}
//-----------------------------------------------------------------------------------------------------------------------POOL END
