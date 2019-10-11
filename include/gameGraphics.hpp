#ifndef _GAME_GRAPHICS_H_
#define _GAME_GRAPHICS_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include "gameLogic.hpp"


#define LOAD_TILESET "sprites/tileset1.png"
#define LOAD_TILESET_ERROR "Error when loading map tileset"
#define LOAD_FONT "fonts/mismo/Mismo Black.ttf"
#define LOAD_FONT_ERROR "Error when loading Mismo Black.ttf"
#define TITLE "I Have No Friends"
#define LEFT_CENTER_X 512
#define RIGHT_CENTER_X 2112
#define CENTER_Y 1408
#define BOUND_MIN_Y 512
#define BOUND_MAX_Y 1408
#define BOUND_X1 1088
#define BOUND_X2 2688
#define FRAMERATE_FONT_SIZE 24
#define FRAMERATE 60
#define TILE_DIM 64
#define EMPTY 98

class GameGraphics {
	public:
		GameLogic &gameLogic;
		sf::RenderWindow window;
		sf::Text framerateText;
		sf::Font font;
		sf::Color color;
    	sf::Texture background,playersTexture[2];
    	std::vector<std::vector<sf::Sprite>> map;
    	sf::View viewLeft, viewRight;
    	sf::RectangleShape border;
		float framerate,blinkTime;
		int screenLimit;
		
		GameGraphics(GameLogic &gl);
		void init();
		void update(float deltaTime);
		void load();
		void drawBackground(int k);
		void buildWindow();

		~GameGraphics();
};

#endif
