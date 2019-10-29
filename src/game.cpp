#include <SFML/Audio.hpp>
#include <iostream>
#include "game.hpp"

using namespace sf;
using namespace std;

Game::Game() : gameAudio(),gameLogic(gameAudio),gameGraphics(gameLogic) {
    run();
}

void Game::run() {
    deltaTime=1.0f/MINUTE;
    Clock clock;
    Event event;
    gameAudio.load();
    int keyCode;
    while(gameGraphics.window.isOpen()&&gameLogic.level<=2)
    {
        keyCode=-1;
        while(gameGraphics.window.pollEvent(event)) {
            if(event.type==Event::Closed)
                gameGraphics.window.close();
            else if(event.type==Event::LostFocus) gameLogic.pause=true;
            else if(event.type==Event::GainedFocus) gameLogic.pause=false;
            else if(event.type==Event::KeyReleased) keyCode=event.key.code;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) break;
        gameLogic.update(deltaTime,gameGraphics.map,keyCode);
        gameGraphics.update(deltaTime);
        
        deltaTime=clock.getElapsedTime().asSeconds();
        clock.restart();
    }
    while(gameGraphics.window.isOpen())
    {
        while(gameGraphics.window.pollEvent(event))
            if(event.type==Event::Closed)
                gameGraphics.window.close();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) break;
    }
}

Game::~Game(){}
