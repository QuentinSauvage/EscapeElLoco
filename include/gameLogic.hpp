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
#define JUMP_VELOCITY 900
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
#define ROPE_TOP 7
#define CHEST_L 30
#define CHEST_R 31
#define KEY 90
#define DOOR_T 74
#define DOOR_B 82
#define TABLE 59
#define CHAIR_L 68
#define CHAIR_R 69
#define BOUQUET 60
#define PLAYER3_SPRITE "sprites/dino/3.png"
#define PLAYER4_SPRITE "sprites/dino/4.png"

class GameLogic {
	public:
		struct Map {
			int height,width;
			std::vector<std::vector<int>> map;
			std::vector<std::vector<int>> collisions;
		};
		struct Modif {
			int x,y,value;
		};
		GameAudio &gameAudio;
		int level,p_index;
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
		void jump(std::vector<std::vector<sf::Sprite>> &gmap,float dt);
		void handleCollisions(std::vector<std::vector<sf::Sprite>> &gmap,float dt);
		//check if the player is on a block that just reappeared, if so, move it on the closest empty tile
		void handleCollisions2(std::vector<std::vector<sf::Sprite>> &gmap, float dt);
		void update(float deltaTime,std::vector<std::vector<sf::Sprite>> &gmap);
		void testCollisions(std::vector<std::vector<sf::Sprite>> &gmap,float dt);
		//-1:makes the player fall, 0: the players can go through (stops the fall), 1: solid object
		int  tileType(int tile);
		bool isFalling(int tile);
		void interact(int &indX,int &indY,float deltaTime);
		bool updateCoin(float deltaTime);
		bool updateTimerBlocks(float deltaTime);

		~GameLogic();
};

#endif