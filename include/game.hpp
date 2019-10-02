#ifndef _GAME_H_
#define _GAME_H_

#include "gameLogic.hpp"
#include "gameGraphics.hpp"

class Game {
	public:
		float deltaTime;
		GameLogic gameLogic;
		GameGraphics gameGraphics;

		Game();

		void run();
		void getFramerate();

		~Game();
};

#endif