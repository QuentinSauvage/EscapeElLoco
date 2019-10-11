#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "entity.hpp"

#define FALLABLE_TIMER 3.0f

class Block : public virtual Entity {
	public:
		bool displayed;
		float timer;
		
		Block();
		Block(bool displayed,float timer,float x,float y,float scale,float rotation);

		~Block();
};

class FallableBlock : public Block {
	public:
		FallableBlock();
		~FallableBlock();
};

class TimerBlock : public Block {
	public:
		float timer;
		TimerBlock(float t);
		~TimerBlock();
};

class CoinBlock : public Block, public SidedEntity {
	public:
		CoinBlock();
		~CoinBlock();
};

#endif