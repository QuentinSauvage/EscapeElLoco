#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

#include <SFML/Graphics.hpp>
#include "gameAudio.hpp"
#include <vector>
#include <string>
#include "player.hpp"
#include "block.hpp"

#define POSY 1695
#define PATH_LEVEL "levels/level"
#define CSV ".csv"
#define LOAD_MAP_ERROR "Error while reading map\n"
#define JUMP_VELOCITY 800
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
#define CHEST_LC 30
#define CHEST_RC 31
#define CHEST_LO 92
#define CHEST_RO 93
#define KEY 90
#define DOOR_TC 74
#define DOOR_BC 82
#define DOOR_TO 77
#define DOOR_BO 85
#define TABLE 59
#define CHAIR_L 68
#define CHAIR_R 69
#define BOUQUET 60
#define PLAYER1_SPRITE "sprites/dino/1.png"
#define PLAYER2_SPRITE "sprites/dino/2.png"
#define PLAYER3_SPRITE "sprites/dino/3.png"
#define PLAYER4_SPRITE "sprites/dino/4.png"
#define TIMER_EL_LOCO_1 10.0f
#define TIMER_EL_LOCO_2 10.4f
#define CHEAT_MESSAGE "You cheated not only the game, but yourself.\n\n\
You didn't grow.\nYou didn't improve.\nYou took a shortcut and gained nothing.\n\n\
You experienced a hollow victory.\n\
Nothing was risked and nothing was gained.\n\n\
It's sad that you don't know the difference."

class GameLogic {
	public:
		struct Map {
			int height,width;
			std::vector<std::vector<int>> map;
			std::vector<std::vector<int>> collisions;
		};
		struct Box {
			unsigned char flags : 6;
			int top,down,left,right,centerX,centerY;
		};
		struct Modif {
			int x,y,value,w,h;
		};
		GameAudio &gameAudio;
		int level,p_index;
		float timer,timerElLoco;
		bool godMode,hardcoreMode,pause,interactEvent,timerBlocksDisplayed,climbing,end,elLocoDisplayed,cheating;
		Map map;
		std::vector<Modif> modifs;
		std::vector<TimerBlock> timerBlocks;
		std::vector<CollapseBlock> collapseBlocks[2];
		std::vector<CollapseBlock> collapsingBlocks;
		sf::Sprite elLoco;
		Coin coins[2];
		Player players[2];

		GameLogic(GameAudio &audio);
		void clear();
		void initLevel();
		void extractMap();
		void addModif(int x,int y,int type,int dimX,int dimY);
		void handleEvents(float deltaTime,int keyCode);
		void jump(float dt);
		void handleCollisions(const std::vector<std::vector<sf::Sprite>> &gmap,float dt);
		void handleCollisions2(const std::vector<std::vector<sf::Sprite>> &gmap, float dt);
		void update(float deltaTime,const std::vector<std::vector<sf::Sprite>> &gmap,int keyCode);
		void testCollisions(const std::vector<std::vector<sf::Sprite>> &gmap,float dt);
		//-1:makes the player fall, 0: the players can go through (stops the fall), 1: solid object
		int  tileType(int tile) const;
		void interact(int indX,int indY,float deltaTime);
		bool updateCoin(float deltaTime);
		bool updateCollapseBlocks(float deltaTime);
		bool updateTimerBlocks(float deltaTime);
		void animateElLoco(float deltaTime);
		void addModif(int indX,int indY,int value);
		void initBox(Box &b) const;
		~GameLogic();
};

#endif