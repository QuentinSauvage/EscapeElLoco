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
#define GRAVITY 50
#define PLAYER_DIM_SCALED 32
#define TILE_DIM 64
#define EMPTY 98
#define PLAYER3_SPRITE "sprites/dino/3.png"
#define PLAYER4_SPRITE "sprites/dino/4.png"

class GameLogic {
	public:
		struct Map {
			int height,width;
			std::vector<std::vector<int>> map;
		};
		GameAudio &gameAudio;
		int level;
		float timer;
		bool pause;
		Map map;
		std::vector<Block> blocks;
		Player players[2];

		GameLogic(GameAudio &audio);
		void increaseLevel();
		void extractMap();
		void handleEvents(float deltaTime);
		void setPlayerSprite(int p_index, sf::Sprite &s);
		void jump(int p_index, std::vector<std::vector<sf::Sprite>> &map, float dt);
		void handleCollisions(int p_index, std::vector<std::vector<sf::Sprite>> &map, float dt);
		void update(float deltaTime, std::vector<std::vector<sf::Sprite>> &map);
		void testCollisions(int p_index, std::vector<std::vector<sf::Sprite>> &map, float dt);

		~GameLogic();
};

#endif