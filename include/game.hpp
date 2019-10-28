#ifndef _GAME_H_
#define _GAME_H_

#include "gameLogic.hpp"
#include "gameGraphics.hpp"
#include "gameAudio.hpp"

#define MINUTE 60.0f

class Game {
	public:
		float deltaTime;
		GameAudio gameAudio;
		GameLogic gameLogic;
		GameGraphics gameGraphics;

		Game();
		void run();
		~Game();
};

#endif