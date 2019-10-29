#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "entity.hpp"

class Coin : public Entity {
	public:
		float timer;
		bool collected;
		Coin();
		Coin(float x,float y,float s,float r,bool b);
		~Coin();
};

class Key : public Entity {
	public:
		bool collected;
		Key();
		Key(float x,float y,float s,float r);
		~Key();
};

#endif