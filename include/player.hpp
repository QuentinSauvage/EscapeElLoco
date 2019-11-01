#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics.hpp>
#include "entity.hpp"
#include "object.hpp"
#include "block.hpp"
#include <string>
#include <vector>

#define LOAD_PLAYER_SPRITE_ERROR "Error when loading character sprite"
#define DEFAULT_SPEED 300.0f
#define CLIMB_SPEED 400.0f
#define PLAYER_DIM 16
#define RUN_1_OFFSET 414
#define RUN_2_OFFSET 462
#define IDLE_1_OFFSET 4
#define IDLE_2_OFFSET 28
#define JUMP_1_OFFSET 266
#define JUMP_2_OFFSET 220
#define JUMP_3_OFFSET 196
#define OFFSET_Y 4
#define TIMER_IDLE 0.7f
#define TIMER_RUN 0.1f

class Player : public Entity {
	public:
		//0:idle, 1:walking, 2:jumping, need enum
		int state;
		float vx,vy,timerIdle,timerRun,originX,originY,oldX;
		bool chestOpened,doorOpened;
		sf::Sprite sprite,oldSprite;
		sf::Texture texture;
		sf::IntRect spriteRect;
		std::vector<CoinBlock> coinBlocks;
		std::vector<Coin> coins;
		Coin coin;
		Key key;

		Player();
		void init(float x,float y,std::string path);
		void move(int dir,float deltaTime);
		void setSprite(sf::Texture &t);
		void animateRun(float t1,float t2,int s,int offset1,int offset2);
		void animate(float timer);
		void climb(bool climbing,float deltaTime);

		~Player();
};

#endif