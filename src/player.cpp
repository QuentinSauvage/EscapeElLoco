#include "player.hpp"
#include <iostream>

Player::Player() : Entity(),life(DEFAULT_MAX_LIFE),maxLife(DEFAULT_MAX_LIFE),speed(DEFAULT_SPEED),coinTaken(false),spriteRect(IDLE_1_OFFSET,OFFSET_Y,PLAYER_DIM,PLAYER_DIM) {
	vx=0,vy=0;
	state=0; //0:idle, 1:walking, 2:jumping, need enum
	timerIdle=0;
	timerRun=0;
} 

void Player::init(float x,float y,std::string path){
	this->x=x;
	this->y=y;
	if(!texture.loadFromFile(path)) {
    	std::cout << LOAD_PLAYER_SPRITE_ERROR << std::endl;
    	exit(1);
    }
}

void Player::setSprite(sf::Texture &t) {
	sprite.setTexture(t);
    sprite.setTextureRect(spriteRect);
	sprite.setOrigin(8,8);
    sprite.setPosition(x,y);
    sprite.setScale(4.f,4.f);
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
			if(timerRun>TIMER_RUN && state==1)
				spriteRect.left=(spriteRect.left == RUN_1_OFFSET)? RUN_2_OFFSET : RUN_1_OFFSET;
			else if(state!=1) {
				state=1;
				spriteRect.left=RUN_1_OFFSET;
			}
			sprite.setScale(-4.f,4.f);
		} else if(vx>0) {
			if(timerRun>TIMER_RUN && state==1)
				spriteRect.left=(spriteRect.left == RUN_1_OFFSET)? RUN_2_OFFSET : RUN_1_OFFSET;
			else if(state!=1) {
				state=1;
				spriteRect.left=RUN_1_OFFSET;
			}
			sprite.setScale(4.f,4.f);
		} else {
			if(timerIdle>TIMER_IDLE && state==0)
				spriteRect.left=(spriteRect.left == IDLE_1_OFFSET)? IDLE_2_OFFSET : IDLE_1_OFFSET;
			else if(state!=0) {
				state=0;
				spriteRect.left=IDLE_1_OFFSET;
			}
		}
	}
	if(state!=0 || timerIdle>TIMER_IDLE) timerIdle=0;
	if(state!=1 || timerRun>TIMER_RUN) timerRun=0;
	sprite.setTextureRect(spriteRect);
}

void Player::move(int dir,float deltaTime) {
	vx=dir;
	x+=dir*speed*deltaTime;
}

Player::~Player() {}