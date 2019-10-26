#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "entity.hpp"

#define COLLAPSE_BLOCK_TIMER_1 0.3f
#define COLLAPSE_BLOCK_TIMER_2 3.0f
#define TIMER_BLOCK_TIMER 2.5f
#define COIN_BLOCK_TIMER 7.0f

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
		CoinBlock(float x,float y,float s,float r);
		~CoinBlock();
};

#endif