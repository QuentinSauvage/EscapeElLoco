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
		int state;
		float speed,vx,vy,timerIdle,timerRun,origin_x,origin_y;
		bool chestOpened,doorOpened;
		sf::Sprite sprite;
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
		void animate(float timer);

		~Player();
};

#endif