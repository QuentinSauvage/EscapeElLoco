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

class Coin : public SidedEntity {
	public:
		Coin();
		~Coin();
};

class Key : public SidedEntity {
	public:
		bool displayed;
		Key(float t);
		~Key();
};

class Chest : public SidedEntity {
	public:
		Chest();
		~Chest();
};

#endif