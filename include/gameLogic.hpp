#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

#include <vector>
#include <string>
#include "player.hpp"
#include "block.hpp"

#define POSX1 512
#define POSX2 2112
#define POSY 1664
#define PATH_LEVEL "levels/level"
#define CSV ".csv"
#define LOAD_MAP_ERROR "Error while reading map\n"

class GameLogic {
	public:
		struct Map {
			int height,width;
			std::vector<std::vector<int>> map;
		};


		int level;
		Map map;
		std::vector<Block> blocks;
		Player players[2];

		GameLogic();
		void increaseLevel();
		void extractMap();
		void handleEvents(float deltaTime);
		void update(float deltaTime);

		~GameLogic();
};

#endif