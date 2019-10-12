#include "block.hpp"
Block::Block() : Entity(),displayed(true),timer(-1) {}
Block::Block(float x,float y,float t,bool d=true,float s=1,float r=0) : Entity(x,y,scale,rotation) {
	displayed=d;
	timer=t;
}
Block::~Block() {}

CollapseBlock::CollapseBlock() : Block() {}
CollapseBlock::CollapseBlock(float x,float y,float t=TIMER_COLLAPSE_TIMER,float s=1,float r=0) : Block(x,y,t,s,r) {}
CollapseBlock::~CollapseBlock() {}

TimerBlock::TimerBlock() : Block() {}
TimerBlock::TimerBlock(float x,float y,float t=TIMER_BLOCK_TIMER,bool d=true,float s=1,float r=0) : Block(x,y,t,d,s,r) {}
TimerBlock::~TimerBlock() {}

CoinBlock::CoinBlock() : Block() {}
CoinBlock::CoinBlock(float x,float y,float t,float s,float r) : Block(x,y,t,s,r) {}
CoinBlock::~CoinBlock() {}