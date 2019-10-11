#ifndef _ENTITY_H_
#define _ENTITY_H_

#define COIN_TIMER 10.0f

class Entity {
	public:
		float x,y,scale,rotation;

		Entity();
		Entity(float x,float y,float scale,float rotation);
		~Entity();
};

class SidedEntity : public virtual Entity {
	public:
		int side;
		SidedEntity();
		~SidedEntity();
};



#endif