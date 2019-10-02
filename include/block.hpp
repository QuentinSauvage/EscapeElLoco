#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "object.hpp"

#define DEFAULT_TIMER 2000

class Block : public Object {
	public:
		bool displayed;
		float timer;
		
		Block();
		Block(bool displayed, float timer, float x, float y, float scale, float rotation);

		~Block();
};

#endif