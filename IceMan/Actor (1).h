#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {

public:
	Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0);
	~Actor();
	//virtual void doSomething() = 0;
};

class Iceman : public Actor {

	//Chris
	int hit_points{ 10 };
	int water_units{ 5 };
	int sonar{ 1 };
	int gold{ 0 };

public:
	Iceman();

	void increaseWater();
	void increaseSonar();
	void increaseGold();
	void decreaseWater();
	void decreaseSonar();
	void decreaseGold();
	void decreaseHit();

	void doSomething();

	~Iceman();
};

class Protester : public Actor {

	//Ethan
public:
};

class HardcoreProtester : public Protester {

	//Ethan
public:
};

class Item : public GraphObject {

public:
	Item(int imageID, int startX, int startY, Direction dir = none, double size = 1.0, unsigned int depth = 0);
	~Item();
};

class Ice : public Item {

public:
	Ice(int startX, int startY);
	~Ice();
};

class Gold : public Item {

public:
	Gold(int startX, int startY);
	~Gold();
};

class Oil : public Item {

public:
	Oil(int startX, int startY);
	~Oil();
};

class Boulder : public Item {

public:
	Boulder(int startX, int startY);
	~Boulder();
};

class Sonar : public Item {

public:
	Sonar(int startX, int startY);
	~Sonar();
};

class Gun : public Item { //Water gun.

public:
	Gun(int startX, int startY);
	~Gun();
};

class Pool : public Item {

public:
	Pool(int startX, int startY);
	~Pool();
};


#endif //ACTOR_H_
