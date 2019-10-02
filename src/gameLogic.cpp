#include <fstream>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "gameLogic.hpp"

using namespace std;

GameLogic::GameLogic() : level(1) {
	extractMap();
	players[0].init(POSX1, POSY);
	players[1].init(POSX2, POSY);
}

void GameLogic::increaseLevel() {
	level++;
}

//extract map from file
void GameLogic::extractMap() {
	ifstream levelMap;
	string filename = PATH_LEVEL;
	char c;
	int tmp;
	filename += to_string(level) + CSV;
	levelMap.open(filename.c_str());

	if(levelMap.good()) {
		levelMap >> map.height >> c >> map.width >> c;
		for(int i = 0; i < map.height; i++) {
			vector<int> v;
			for(int j = 0; j < map.width; j++) {
				levelMap >> tmp >> c;
				v.push_back(tmp);
			}
			map.map.push_back(v);
		}
		levelMap.close();
	} else cerr << LOAD_MAP_ERROR;
}

//handle inputs from the user
void GameLogic::handleEvents(float deltaTime) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
	    players[0].move(-1, deltaTime);
	    players[1].move(-1, deltaTime);
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		players[0].move(1, deltaTime);
		players[1].move(1, deltaTime);
	}
}

void GameLogic::update(float deltaTime) {
	handleEvents(deltaTime);
}

GameLogic::~GameLogic(){}
