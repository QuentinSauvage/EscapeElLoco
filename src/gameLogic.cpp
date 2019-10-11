#include <fstream>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "gameLogic.hpp"

using namespace std;

GameLogic::GameLogic(GameAudio &audio) : gameAudio(audio),level(1),timer(0),pause(false),interactEvent(false) {
	extractMap();
	players[0].init(POSX1,POSY,PLAYER3_SPRITE);
	players[1].init(POSX2,POSY,PLAYER4_SPRITE);
}

bool GameLogic::isSolid(int tile,int p_index) {
	if(players[p_index].coinTaken)
		return false;
	return tile==EMPTY||tile==COIN;
}

void GameLogic::interact(int indX, int indY) {
	for(int i=0;i<=1;i++) {
		cout << map.map[indY][indX] << endl;
	}
	interactEvent=false;
}

//extract map from file
void GameLogic::extractMap() {
	ifstream levelMap;
	string filename=PATH_LEVEL;
	char c;
	int tmp;
	filename+=to_string(level)+CSV;
	levelMap.open(filename.c_str());

	if(levelMap.good()) {
		levelMap >> map.height >> c >> map.width >> c;
		for(int i=0;i<map.height;i++) {
			vector<int> v;
			for(int j=0;j<map.width;j++) {
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
	if(!pause) {
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			players[0].move(-1,deltaTime);
			players[1].move(-1,deltaTime);
		} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			players[0].move(1,deltaTime);
			players[1].move(1,deltaTime);
		} else {
			players[0].vx=0;
			players[1].vx=0;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			if(players[0].state!=2) {
				players[0].vy=JUMP_VELOCITY;
				players[0].state=2;
				if(gameAudio.playing) gameAudio.jumpSoundLeft.play();
			}
			if(players[1].state!=2) {
				players[1].vy=JUMP_VELOCITY;
				players[1].state=2;
				if(gameAudio.playing) gameAudio.jumpSoundRight.play();
			}
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) gameAudio.changeState();
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) interactEvent = true;
	}
}

void GameLogic::jump(int p_index,vector<vector<sf::Sprite>> &gmap,float deltaTime) {
	if(players[p_index].state==2) {
        players[p_index].vy-=(GRAVITY*deltaTime);
		players[p_index].y-=players[p_index].vy*deltaTime;
		players[p_index].sprite.setPosition(players[p_index].x,players[p_index].y);
    }
}

void GameLogic::handleCollisions(int p_index,vector<vector<sf::Sprite>> &gmap,float deltaTime) {
	jump(p_index,gmap,deltaTime);
	float xp,xn,yp,yn;
	int indY=players[p_index].y/TILE_DIM,indX=players[p_index].x/TILE_DIM;
	sf::FloatRect rect,r=players[p_index].sprite.getGlobalBounds();

	if(interactEvent) interact(indX,indY);
	if(isSolid(map.map[indY+1][indX],p_index) && players[p_index].state!=2) {
		players[p_index].state=2;
		players[p_index].vy=0;
	}

	if(players[p_index].vy>0) {
		yn = indY;
		yp = (indY+4>map.height)? map.height : indY+4;
	} else if(players[p_index].vy<0) {
		yn=(indY-4<0)? 0 : indY-4;
		yp=indY+1;
	} else yn=yp=0;

	for(;yn<yp;yn++)
		if(!isSolid(map.map[yn][indX],p_index) && (players[p_index].sprite.getGlobalBounds().intersects(gmap[yn][indX].getGlobalBounds(),rect))) {
			if(rect.top>r.top) {
				players[p_index].y-=rect.height;
				players[p_index].state=(players[p_index].vx!=0)? 1 : 0;
				players[p_index].spriteRect.left=IDLE_1_OFFSET;
			} else players[p_index].y+=rect.height;
			players[p_index].vy=0;
			players[p_index].sprite.setPosition(players[p_index].x,players[p_index].y);
			r = players[p_index].sprite.getGlobalBounds();
			break;
		}
	xn=(indX-4<0)? 0 : indX-4;
	xp=(indX+4>map.width)? map.width : indX+4;
	for(;xn<xp;xn++)
		if(!isSolid(map.map[indY][xn],p_index) && (players[p_index].sprite.getGlobalBounds().intersects(gmap[indY][xn].getGlobalBounds(),rect))) {
			if(rect.left>r.left) players[p_index].x-=rect.width;
			else players[p_index].x+=rect.width;
			players[p_index].sprite.setPosition(players[p_index].x,players[p_index].y);
			break;
		}
}

void GameLogic::update(float deltaTime,vector<std::vector<sf::Sprite>> &map) {
	handleEvents(deltaTime);
	handleCollisions(0,map,deltaTime);
	handleCollisions(1,map,deltaTime);

	players[0].animate(deltaTime);
	players[1].animate(deltaTime);

	players[0].sprite.setPosition(players[0].x,players[0].y);
	players[1].sprite.setPosition(players[1].x,players[1].y);
}

GameLogic::~GameLogic(){}
