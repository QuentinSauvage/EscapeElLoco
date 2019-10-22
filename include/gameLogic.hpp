#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

#include <SFML/Graphics.hpp>
#include "gameAudio.hpp"
#include <vector>
#include <string>
#include "player.hpp"
#include "block.hpp"

#define POSX1 512
#define POSX2 2112
#define POSY 1695
#define PATH_LEVEL "levels/level"
#define CSV ".csv"
#define LOAD_MAP_ERROR "Error while reading map\n"
#define JUMP_VELOCITY 1000
#define GRAVITY 2500
#define PLAYER_DIM_SCALED 32
#define TILE_DIM 64
#define OFFSET 32
#define EMPTY 98
#define COIN 65
#define COIN_BLOCK 25
#define TIMER_BLOCK 12
#define COLLAPSE_BLOCK 66
#define LADDER 56
#define ROPE 15
#define ROPE_END 23
#define PLAYER3_SPRITE "sprites/dino/3.png"
#define PLAYER4_SPRITE "sprites/dino/4.png"

class GameLogic {
	public:
		struct Map {
			int height,width;
			std::vector<std::vector<int>> map;
		};
		struct Modif {
			int x,y,value;
		};
		GameAudio &gameAudio;
		int level;
		float timer;
		bool pause,interactEvent,timerBlocksDisplayed,climbing;
		Map map;
		std::vector<Modif> modifs;
		std::vector<TimerBlock> timerBlocks;
		std::vector<CollapseBlock> collapseBlocks;
		Coin coins[2];
		Player players[2];

		GameLogic(GameAudio &audio);
		void extractMap();
		void handleEvents(float deltaTime);
		void jump(int p_index,std::vector<std::vector<sf::Sprite>> &gmap,float dt);
		void handleCollisions(int p_index,std::vector<std::vector<sf::Sprite>> &gmap,float dt);
		//check if player is on a block that just reappeared, if so, move it on the closest empty tile
		void handleCollisions2(int p_index,std::vector<std::vector<sf::Sprite>> &gmap, float dt);
		void update(float deltaTime,std::vector<std::vector<sf::Sprite>> &gmap);
		void testCollisions(int p_index,std::vector<std::vector<sf::Sprite>> &gmap,float dt);
		bool isSolid(int tile);
		bool isFalling(int tile);
		void interact(int p_index,int &indX,int &indY,float deltaTime);
		bool updateCoin(int p_index,float deltaTime);
		bool updateTimerBlocks(float deltaTime);

		~GameLogic();
};

#endif