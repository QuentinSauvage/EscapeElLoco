#include "gameGraphics.hpp"
#include <math.h>
#include <iostream>

using namespace sf;

GameGraphics::GameGraphics(GameLogic &gl) : gameLogic(gl) {
	buildWindow();
}

void GameGraphics::loadTextures() {
    if (!playersTexture[0].loadFromFile(PLAYER3_SPRITE)) {
        std::cerr << LOAD_PLAYER_SPRITE_ERROR << std::endl;
        exit(1);
    }
    if (!playersTexture[1].loadFromFile(PLAYER4_SPRITE))
        std::cerr <<  LOAD_PLAYER_SPRITE_ERROR << std::endl;

    if(!background.loadFromFile(LOAD_TILESET)) {
        std::cerr << LOAD_TILESET_ERROR << std::endl;
        exit(1);
    }
    sf::Sprite s;
    for(int i = 0; i < gameLogic.map.height; i++) {
        std::vector<Sprite> v;
        for(int j = 0; j < gameLogic.map.width; j++) {
            s.setTexture(background);
            int tileNumber = gameLogic.map.map[i][j];
            if(tileNumber >= 0) {
                s.setTextureRect(sf::IntRect((tileNumber % 8) << 4, (tileNumber >> 3) << 4, 16, 16));
                s.setPosition(j << 6, i << 6);
                s.setScale(4.f,4.f);
                v.push_back(s);
            }
        }
        map.push_back(v);
    }
}

void GameGraphics::drawBackground(int i) {
    int begin, end;
    if(i) {
        begin = lim;
        end = gameLogic.map.width;
    } else {
        begin = 0;
        end = lim;
    }
    for(int i = 0; i < gameLogic.map.height; i++)
        for(int j = begin; j < end; j++)
            window.draw(map[i][j]);
}

void GameGraphics::buildWindow() {
	VideoMode screen = VideoMode::getDesktopMode();
    window.create(VideoMode(screen.width * 0.75, screen.height * 0.75), TITLE, Style::Titlebar | Style::Close);
    window.setPosition(Vector2i((screen.width >> 1) - (window.getSize().x >> 1), (screen.height >> 1) - (window.getSize().y >> 1)));
	window.setFramerateLimit(60);

    if(!font.loadFromFile(LOAD_FONT)) {
        std::cerr << LOAD_FONT_ERROR << std::endl;
        return;
    }

    loadTextures();

    //blinkTime = BLINK_RATE;

    for(int i = 0; i < 2; i++) {
        playersSprite[i].setTexture(playersTexture[i]);
        playersSprite[i].setTextureRect(sf::IntRect(0, 0, PLAYER_DIM, PLAYER_DIM));
        playersSprite[i].setPosition(gameLogic.players[i].x, gameLogic.players[i].y);
        playersSprite[i].setScale(3.f,3.f);
    }

    framerateText.setFont(font);
    framerateText.setCharacterSize(FRAMERATE_FONT_SIZE);
    framerateText.setFillColor(Color::White);
    framerateText.setOutlineThickness(1);
    framerateText.setOutlineColor(Color::Blue);
    framerateText.setPosition(window.getSize().x - 100, 0);

    float sizeX = (window.getSize().x * 0.004f);
	border.setPosition((window.getSize().x / 2) - (sizeX / 2),0);
    border.setSize(sf::Vector2f(sizeX,window.getSize().y));
    border.setFillColor(Color(192,192,192));

    //setCenter: x: clamp(player.x,8,17), y: clamp(player.y,8,22)
    viewLeft.setCenter(LEFT_CENTER_X, CENTER_Y);
    viewLeft.setViewport(FloatRect(0.f, 0.f, 0.498f, 1.f));
    //setCenter: x: clamp(player.x,33,42), y: clamp(player.y,8,22)
    viewRight.setCenter(RIGHT_CENTER_X, CENTER_Y);
    viewRight.setViewport(FloatRect(0.501f, 0.f, 0.498f, 1.f));

    lim = gameLogic.map.width >> 1;
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
    viewLeft.setCenter(clamp(gameLogic.players[0].x,LEFT_CENTER_X,BOUND_X1), clamp(gameLogic.players[0].y,BOUND_MIN_Y, BOUND_MAX_Y));
    viewRight.setCenter(clamp(gameLogic.players[1].x,RIGHT_CENTER_X,BOUND_X2), clamp(gameLogic.players[1].y,BOUND_MIN_Y, BOUND_MAX_Y));
    window.setView(window.getDefaultView());
    window.clear(Color::Black);    

    window.setView(viewLeft);
    window.draw(playersSprite[0]);
    drawBackground(0);

    window.setView(viewRight);
    window.draw(playersSprite[1]);
    drawBackground(1);
    
    window.setView(window.getDefaultView());
    window.draw(framerateText);
    window.draw(border);
    window.display();
}

GameGraphics::~GameGraphics() {}
