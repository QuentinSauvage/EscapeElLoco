#include "game.hpp"

using namespace sf;
using namespace std;

#define MINUTE 60.0f

Game::Game() : gameLogic(), gameGraphics(gameLogic) {
    run();
}

void Game::run() {
    deltaTime = 1.0f / MINUTE;
    Clock clock;
    Event event;
    while (gameGraphics.window.isOpen())
    {
        while (gameGraphics.window.pollEvent(event))
            if (event.type == Event::Closed)
                gameGraphics.window.close();

        gameLogic.update(deltaTime);
        gameGraphics.update(deltaTime);
        
        deltaTime = clock.getElapsedTime().asSeconds();
        clock.restart();
    }
}

Game::~Game(){}