#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "entity.hpp"

class Door : public Entity {
	public:		
		Door();
		~Door();
};

class Rope : public Entity {
	public:
		Rope();
		~Rope();
};

class Coin : public Entity {
	public:
		float timer;
		bool collected;
		Coin();
		Coin(float x,float y,float s,float r);
		~Coin();
};

class Key : public Entity {
	public:
		bool collected;
		Key();
		Key(float x,float y,float s,float r);
		~Key();
};

class Chest : public Entity {
	public:
		Chest();
		~Chest();
};

#endif