#ifndef ACTOR_H_
#define ACTOR_H_

#include <queue>
#include <chrono>

#include "GraphObject.h"
class StudentWorld; // This is an incomplete type. Also known as a forward declaration.

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
private:
	//StudentWorld* SW;
public:
	Actor(int imageID, int startX, int startY, StudentWorld* world, Direction dir = right, double size = 1.0, unsigned int depth = 0);
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
	int getHit();
	int getWater();
	int getSonar();
	int getGold();

	void doSomething();

	~Iceman();
};

class Protester : public Actor {

	//Ethan
public:
	 Protester(int imageID, int startX, int startY, Direction dir = left, double size = 1.0, unsigned int depth = 0);

	 void doSomething(

	 );

	~Protester();
};

class HardcoreProtester : public Protester {

	//Ethan
public:
	HardcoreProtester();

	~HardcoreProtester();
};

class Item : public GraphObject {

public:
	Item(int imageID, int startX, int startY, Direction dir = none, double size = 1.0, unsigned int depth = 0);
	virtual ~Item();
	
	virtual bool isGrabbable() = 0;
	virtual void setGrabbable(bool) = 0;
	virtual void setFall() = 0;
	virtual void terminate() = 0;
};

class Ice : public Item {

public:
	Ice(int startX, int startY);
	virtual ~Ice();

	bool isGrabbable();
	void setGrabbable(bool val);
	void setFall();
	void terminate();
};

class Gold : public Item {
	bool is_available{ true };

public:
	Gold(int startX, int startY);
	virtual ~Gold();

	bool isGrabbable();
	void setGrabbable(bool val);
	void setFall();
	void terminate();
};

class Oil : public Item {
	bool is_available{ true };

public:
	Oil(int startX, int startY);
	virtual ~Oil();

	bool isGrabbable();
	void setGrabbable(bool val);
	void setFall();
	void terminate();
};

class Boulder : public Item {
	bool it_fell{ false };
	std::chrono::steady_clock::time_point start;
public:
	Boulder(int startX, int startY);
	virtual ~Boulder();

	bool isGrabbable();
	void setGrabbable(bool val);
	void setFall();
	void terminate();
};

class Sonar : public Item {
	bool is_available{ true };

public:
	Sonar(int startX, int startY);
	virtual ~Sonar();

	bool isGrabbable();
	void setGrabbable(bool val);
	void setFall();
	void terminate();
};

class Gun : public Item { //Water gun.
	std::chrono::steady_clock::time_point start;
	
public:
	Gun(int startX, int startY);
	virtual ~Gun();

	bool isGrabbable();
	void setGrabbable(bool val);
	void setFall();
	void terminate();
};

class Pool : public Item {
	bool is_available{ true };
public:
	Pool(int startX, int startY);
	virtual ~Pool();

	bool isGrabbable();
	void setGrabbable(bool val);
	void setFall();
	void terminate();
};


#endif //ACTOR_H_
