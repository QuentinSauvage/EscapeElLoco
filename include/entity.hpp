#ifndef _ENTITY_H_
#define _ENTITY_H_

#define TIMER_COIN_TIMER 5.0f

class Entity {
	public:
		float x,y,scale,rotation;

		Entity();
		Entity(float x,float y,float scale,float rotation);
		~Entity();
};

#endif