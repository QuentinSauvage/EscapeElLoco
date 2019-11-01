#include "player.hpp"
#include <iostream>

Player::Player() : Entity(),chestOpened(false),doorOpened(false),spriteRect(IDLE_1_OFFSET,OFFSET_Y,PLAYER_DIM,PLAYER_DIM) {} 

void Player::init(float x,float y,std::string path){
	this->x=x;
	this->y=y;
	originX=x;
	originY=y;
	vx=vy=state=timerIdle=timerRun=0;
	if(!texture.loadFromFile(path)) {
    	std::cerr << LOAD_PLAYER_SPRITE_ERROR << std::endl;
    	exit(1);
    }
}

void Player::setSprite(sf::Texture &t) {
	sprite.setTexture(t);
    sprite.setTextureRect(spriteRect);
	sprite.setOrigin(8,8);
    sprite.setPosition(x,y);
    sprite.setScale(4.f,4.f);
	oldSprite=sprite;
}

void Player::animateRun(float t1,float t2,int s,int offset1,int offset2) {
	if(t1>t2 && state==s)
		spriteRect.left=(spriteRect.left == offset1)? offset2 : offset1;
	else if(state!=s) {
		state=s;
		spriteRect.left=offset1;
	}
}

void Player::animate(float timer) {
	timerIdle+=timer;
	timerRun+=timer;
	if(state==2) {
		if(vy<-150) spriteRect.left=JUMP_2_OFFSET;
		else if(vy>150) spriteRect.left=JUMP_1_OFFSET;
		else spriteRect.left=JUMP_3_OFFSET;
		if(vx<0) sprite.setScale(-4.f,4.f);
		else if(vx>0) sprite.setScale(4.f,4.f);
	} else {
		if(vx<0) {
			animateRun(timerRun,TIMER_RUN,1,RUN_1_OFFSET,RUN_2_OFFSET);
			sprite.setScale(-4.f,4.f);
		} else if(vx>0) {
			animateRun(timerRun,TIMER_RUN,1,RUN_1_OFFSET,RUN_2_OFFSET);
			sprite.setScale(4.f,4.f);
		} else {
			animateRun(timerIdle,TIMER_IDLE,0,IDLE_1_OFFSET,IDLE_2_OFFSET);
		}
	}
	if(state!=0||timerIdle>TIMER_IDLE) timerIdle=0;
	if(state!=1||timerRun>TIMER_RUN) timerRun=0;
	sprite.setTextureRect(spriteRect);
}

void Player::climb(bool climbing,float deltaTime) {
	vy=0;
	state=2;
	if(climbing) y-=CLIMB_SPEED*deltaTime;
	else y+=CLIMB_SPEED*deltaTime;
}

void Player::move(int dir,float deltaTime) {
	vx=dir;
	oldX=x;
	x+=dir*DEFAULT_SPEED*deltaTime;
	if(dir) oldX=x-oldX;
	else oldX-=x;
}

Player::~Player() {}