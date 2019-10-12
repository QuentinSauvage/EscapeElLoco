#include "entity.hpp"
Entity::Entity() : x(0),y(0),scale(1),rotation(0) {}

Entity::Entity(float x,float y,float scale,float rotation) {
	this->x=x;
	this->y=y;
	this->scale=scale;
	this->rotation=rotation;
}

Entity::~Entity() {}