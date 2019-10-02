#include "object.hpp"

Object::Object() : x(0), y(0), scale(1), rotation(0) {}

Object::Object(float x, float y, float scale, float rotation) {
	this->x = x;
	this->y = y;
	this->scale = scale;
	this->rotation = rotation;
}

Object::~Object() {}