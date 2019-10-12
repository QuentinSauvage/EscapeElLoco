#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "entity.hpp"

#define TIMER_COLLAPSE_TIMER 3.0f
#define TIMER_BLOCK_TIMER 2.0f

class Block : public Entity {
	public:
		bool displayed;
		float timer;
		
		Block();
		Block(float x,float y,float t,bool d,float s,float r);

		~Block();
};

class CollapseBlock : public Block {
	public:
		CollapseBlock();
		CollapseBlock(float x,float y,float t,float s,float r);
		~CollapseBlock();
};

class TimerBlock : public Block {
	public:
		TimerBlock();
		TimerBlock(float x,float y,float t,bool d,float s,float r);
		~TimerBlock();
};

class CoinBlock : public Block {
	public:
		CoinBlock();
		CoinBlock(float x,float y,float t,float s,float r);
		~CoinBlock();
};

#endif