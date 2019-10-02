#include <fstream>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "gameLogic.hpp"

using namespace std;

GameLogic::GameLogic() : level(1) {
	extractMap();
	players[0].x = 8 * 64;
	players[0].y = 26 * 64;
	players[1].x = 33 * 64;
	players[1].y = 26 * 64;
}

void GameLogic::increaseLevel() {
	level++;
}

//extract map from file
void GameLogic::extractMap() {
	ifstream levelMap;
	string filename = "levels/level";
	char c;
	int tmp;
	filename += to_string(level) + ".csv";
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
	} else cerr << "Error while reading map\n";
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
