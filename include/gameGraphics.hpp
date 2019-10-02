#ifndef _GAME_GRAPHICS_H_
#define _GAME_GRAPHICS_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include "gameLogic.hpp"
#include "tileMap.hpp"

#define BLINK_RATE 2000.f

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
		
		GameGraphics(GameLogic &gl);
		void update(float deltaTime);
		void loadTextures();
		void drawBackground();
		void buildWindow();

		~GameGraphics();
};

#endif
