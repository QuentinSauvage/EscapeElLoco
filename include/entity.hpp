#ifndef _ENTITY_H_
#define _ENTITY_H_

class Entity {
	public:
		float x,y,scale,rotation;

		Entity();
		Entity(float x,float y,float scale,float rotation);
		~Entity();
};

#endif