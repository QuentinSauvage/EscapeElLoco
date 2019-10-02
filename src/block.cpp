#include "block.hpp"

Block::Block() : Object(), displayed(false), timer(10) {}
Block::Block(bool displayed, float timer, float x, float y, float scale=1, float rotation=0) : Object(x,y,scale,rotation) {
	this->displayed = displayed;
	this->timer = timer;
}
Block::~Block() {}