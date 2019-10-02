#ifndef _GAME_GRAPHICS_H_
#define _GAME_GRAPHICS_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include "gameLogic.hpp"

#define PLAYER3_SPRITE "sprites/dino/3.png"
#define PLAYER4_SPRITE "sprites/dino/4.png"
#define LOAD_PLAYER_SPRITE_ERROR "Error when loading character sprite"
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
#define PLAYER_DIM 24

class GameGraphics {
	public:
		GameLogic &gameLogic;
		sf::RenderWindow window;
		sf::Text framerateText;
		sf::Font font;
		sf::Color color;
    	sf::Texture playersTexture[2];
    	sf::Texture background;
    	std::vector<std::vector<sf::Sprite>> map;
    	sf::Sprite playersSprite[2];
    	sf::View viewLeft, viewRight;
    	sf::RectangleShape border;
		float framerate;
		float blinkTime;
		int lim;
		
		GameGraphics(GameLogic &gl);
		void update(float deltaTime);
		void loadTextures();
		void drawBackground(int i);
		void buildWindow();

		~GameGraphics();
};

#endif
