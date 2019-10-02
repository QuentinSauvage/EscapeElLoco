#include "gameGraphics.hpp"
#include <math.h>
#include <iostream>

using namespace sf;

GameGraphics::GameGraphics(GameLogic &gl) : gameLogic(gl) {
	buildWindow();
}

void GameGraphics::loadTextures() {
    if (!playersTexture[0].loadFromFile("sprites/dino/3.png"))
    {
        std::cerr << "Error when loading character sprite" << std::endl;
        return;
    }
    if (!playersTexture[1].loadFromFile("sprites/dino/4.png"))
    {
        std::cerr << "Error when loading character sprite" << std::endl;
        return;
    }
    //castle-tileset : 16x16
    //freetileset : 128x128
    //Nature Platformer : 16x16
    //Retro_pixel_plains : 8x8

    if(!background.loadFromFile("sprites/tileset1.png")) {
        std::cerr << "Error when loading map tileset" << std::endl;
        return;
    }
    sf::Sprite s;
    for(int i = 0; i < gameLogic.map.height; i++) {
        std::vector<Sprite> v;
        for(int j = 0; j < gameLogic.map.width; j++) {
            s.setTexture(background);
            int tileNumber = gameLogic.map.map[i][j];
            if(tileNumber == -1)
                continue;
            s.setTextureRect(sf::IntRect((tileNumber % 8) * 16, (tileNumber / 8) * 16, 16, 16));
            s.setPosition(j * 64, i * 64);
            s.setScale(4.f,4.f);
            v.push_back(s);
        }
        map.push_back(v);
    }
}

//viewLeft and viewRight should only draw their part
void GameGraphics::drawBackground() {
    for(int i = 0; i < gameLogic.map.height; i++)
        for(int j = 0; j < gameLogic.map.width; j++)
            window.draw(map[i][j]);
}

void GameGraphics::buildWindow() {
	VideoMode screen = VideoMode::getDesktopMode();
    window.create(VideoMode(screen.width * 0.75, screen.height * 0.75), "Top Secret");
    window.setPosition(Vector2i((screen.width / 2) - (window.getSize().x / 2), (screen.height / 2) - (window.getSize().y / 2)));
	window.setFramerateLimit(60);

    if(!font.loadFromFile("fonts/mismo/Mismo Black.ttf")) {
        std::cerr << "Error when loading Mismo Black.ttf" << std::endl;
        return;
    }

    loadTextures();

    blinkTime = BLINK_RATE;

    playersSprite[0].setTexture(playersTexture[0]);
    playersSprite[0].setTextureRect(sf::IntRect(0, 0, 24, 24));
    playersSprite[0].setPosition(gameLogic.players[0].x, gameLogic.players[0].y);
    playersSprite[0].setScale(3.f,3.f);

    playersSprite[1].setTexture(playersTexture[1]);
    playersSprite[1].setTextureRect(sf::IntRect(0, 0, 24, 24));
    playersSprite[1].setPosition(gameLogic.players[1].x, gameLogic.players[1].y);
    playersSprite[1].setScale(3.f,3.f);

    framerateText.setFont(font);
    framerateText.setCharacterSize(24);
    framerateText.setFillColor(Color::White);
    framerateText.setOutlineThickness(1);
    framerateText.setOutlineColor(Color::Blue);
    framerateText.setPosition(window.getSize().x - 100, 0);

    float sizeX = (window.getSize().x * 0.004f);
	border.setPosition((window.getSize().x / 2) - (sizeX / 2),0);
    border.setSize(sf::Vector2f(sizeX,window.getSize().y));
    border.setFillColor(Color(192,192,192));

    //setCenter: x: clamp(player.x,8,17), y: clamp(player.y,8,22)
    viewLeft.setCenter(8*64, 22*64);
    viewLeft.setViewport(FloatRect(0.f, 0.f, 0.498f, 1.f));
    //setCenter: x: clamp(player.x,33,42), y: clamp(player.y,8,22)
    viewRight.setCenter(33*64, 22*64);
    viewRight.setViewport(FloatRect(0.501f, 0.f, 0.498f, 1.f));
}

float clamp(float v, float min, float max) {
    if(v < min)
        return min;
    if(v > max)
        return max;
    return v; 
}

void GameGraphics::update(float deltaTime) {
	framerate = 1.f / (deltaTime);

	framerateText.setString(std::to_string((int) ceil(framerate)) + " fps");

	/*if(blinkTime <= 0) {
    	blinkTime = BLINK_RATE;
    } else blinkTime -= (deltaTime * 1000.f);*/

	playersSprite[0].setPosition(gameLogic.players[0].x, gameLogic.players[0].y);
    playersSprite[1].setPosition(gameLogic.players[1].x, gameLogic.players[1].y);

    //Draw
    viewLeft.setCenter(clamp(gameLogic.players[0].x,8*64,17*64), clamp(gameLogic.players[0].y,8*64,22*64));
    viewRight.setCenter(clamp(gameLogic.players[1].x,33*64,42*64), clamp(gameLogic.players[1].y,8*64,22*64));
    window.setView(window.getDefaultView());
    window.clear(Color::Black);    

    window.setView(viewLeft);
    window.draw(playersSprite[0]);
    drawBackground();

    window.setView(viewRight);
    window.draw(playersSprite[1]);
    drawBackground();
    
    window.setView(window.getDefaultView());
    window.draw(framerateText);
    window.draw(border);
    window.display();
}

GameGraphics::~GameGraphics() {}
