#include <fstream>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "gameLogic.hpp"

using namespace std;

GameLogic::GameLogic(GameAudio &audio) : gameAudio(audio),level(1),timer(0),pause(false),interactEvent(false),timerBlocksDisplayed(false) {
	extractMap();
	players[0].init(POSX1,POSY,PLAYER3_SPRITE);
	players[1].init(POSX2,POSY,PLAYER4_SPRITE);
}

bool GameLogic::isFalling(int tile) {
	return tile==EMPTY||tile==COIN;
}

bool GameLogic::isSolid(int tile) {
	return tile!=EMPTY&&tile!=COIN&&tile!=LADDER&&tile!=ROPE&&tile!=ROPE_END
	&&tile!=DOOR_B&&tile!=DOOR_T&&tile!=KEY&&tile!=CHEST_L&&tile!=CHEST_R;
}

void GameLogic::interact(int p_index,int &indX, int &indY,float deltaTime) {
	Modif m;
	if(map.map[indY][indX]==COIN) {
		if(players[p_index].coin.timer>0) {
			map.map[players[p_index].coin.y][players[p_index].coin.x]=COIN;
			m={(int)players[p_index].coin.x,(int)players[p_index].coin.y,EMPTY};
			modifs.push_back(m);
			players[p_index].coin.timer=0;
		}
		if(gameAudio.playing) gameAudio.timer.play();
		players[p_index].coin.x=indX;
		players[p_index].coin.y=indY;
		players[p_index].coin.collected=true;
		map.map[indY][indX]=EMPTY;
		m={indX,indY,EMPTY};
		modifs.push_back(m);
		int x,y,other=(p_index+1)%2;
		size_t size=players[other].coinBlocks.size();
		for(size_t i=0;i<size;i++) {
			x=players[other].coinBlocks[i].x;
			y=players[other].coinBlocks[i].y;
			map.map[x][y]=EMPTY;
			map.collisions[x][y]=false;
			m={y,x,EMPTY};
			modifs.push_back(m);
		}
	} else if(map.map[indY][indX]==LADDER||map.map[indY+1][indX]==LADDER||map.map[indY][indX]==ROPE||
			map.map[indY+1][indX]==ROPE||map.map[indY][indX]==ROPE_END||map.map[indY+1][indX]==ROPE_END) {
		players[p_index].vy=0;
		players[p_index].state=2;
		float tmp=players[p_index].y;

		if(climbing) players[p_index].y-=players[p_index].speed*deltaTime;
		else players[p_index].y+=players[p_index].speed*deltaTime;
		int y=(players[p_index].y-OFFSET)/TILE_DIM;
		if(map.collisions[y][indX]) players[p_index].y=tmp;
		else indY=y;
	}
}

//extract map from file
void GameLogic::extractMap() {
	ifstream levelMap;
	string filename=PATH_LEVEL;
	char c;
	int tmp;
	filename+=to_string(level)+CSV;
	levelMap.open(filename.c_str());

	vector<CoinBlock> coinBlocks;
	if(levelMap.good()) {
		levelMap >> map.height >> c >> map.width >> c;
		int sep=map.width>>1;
		for(int i=0;i<map.height;i++) {
			vector<int> v;
			vector<bool> t;
			for(int j=0;j<map.width;j++) {
				levelMap >> tmp >> c;
				v.push_back(tmp);
				t.push_back(isSolid(tmp));
				if(tmp==COIN_BLOCK) {
					if(j>sep) players[1].coinBlocks.push_back(CoinBlock(i,j,1,0));
					else players[0].coinBlocks.push_back(CoinBlock(i,j,1,0));
				} else if(tmp==COIN) {
					if(j>sep) players[1].coins.push_back(Coin());
					else players[0].coins.push_back(Coin());
				} else if(tmp==TIMER_BLOCK) {
					timerBlocks.push_back(TimerBlock(i,j,TIMER_BLOCK_TIMER,true,1,0));
				} else if(tmp==COLLAPSE_BLOCK) {
					collapseBlocks.push_back(CollapseBlock(i,j,10.f,1,0));
				}
			}
			map.map.push_back(v);
			map.collisions.push_back(t);
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
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			players[0].x=POSX1;
			players[1].x=POSX2;
			players[0].y=players[1].y=POSY;
			players[0].state=players[1].state=0;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) gameAudio.changeState();
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			climbing=true;
			interactEvent = true;
		} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			climbing=false;
			interactEvent = true;
		}
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
	float xp,xn,yp,yn;
	int indY=players[p_index].y/TILE_DIM,indX=players[p_index].x/TILE_DIM;
	sf::FloatRect rect,r=players[p_index].sprite.getGlobalBounds();

	if(interactEvent) interact(p_index,indX,indY,deltaTime);
	if(!map.collisions[indY+1][indX] && players[p_index].state!=2) {
		if(isFalling(map.map[indY+1][indX])) players[p_index].state=2;
		else {players[p_index].state=0;players[p_index].vy=0;}
	}
	if(players[p_index].vx>0) {
		xp=(indX+2>map.width)? map.width : indX+2;
		xn=indX;
	} else {
		xp=indX;
		xn=(indX-2<0)? 0 : indX-2;
	}

	for(;xn<xp;xn++)
		if(map.collisions[indY][xn] && (players[p_index].sprite.getGlobalBounds().intersects(gmap[indY][xn].getGlobalBounds(),rect))) {
			if(rect.left>r.left) players[p_index].x-=rect.width;
			else players[p_index].x+=rect.width;
			players[p_index].sprite.setPosition(players[p_index].x,players[p_index].y);
			break;
		}

	jump(p_index,gmap,deltaTime);

	indY=players[p_index].y/TILE_DIM,indX=players[p_index].x/TILE_DIM;
	if(players[p_index].vy>=0) {
		yn=(indY-2<0)? 0 : indY-2;
		yp=indY+1;
	} else {
		yn = indY;
		yp = (indY+2>map.height)? map.height : indY+2;
	}
	for(;yn<yp;yn++)
		if(map.collisions[yn][indX] && (players[p_index].sprite.getGlobalBounds().intersects(gmap[yn][indX].getGlobalBounds(),rect))) {
			if(rect.top>=r.top) {
				players[p_index].y-=rect.height;
				players[p_index].state=(players[p_index].vx!=0)? 1 : 0;
				players[p_index].spriteRect.left=IDLE_1_OFFSET;
			} else players[p_index].y+=rect.height;
			players[p_index].vy=0;
			players[p_index].sprite.setPosition(players[p_index].x,players[p_index].y);
			r = players[p_index].sprite.getGlobalBounds();
		}
}

void GameLogic::handleCollisions2(int p_index,vector<vector<sf::Sprite>> &gmap, float deltaTime) {
	int indY=(players[p_index].y-OFFSET)/TILE_DIM,indX=players[p_index].x/TILE_DIM;
	sf::FloatRect rect;
	if(map.collisions[indY][indX] && (players[p_index].sprite.getGlobalBounds().intersects(gmap[indY][indX].getGlobalBounds(),rect))) {
		for(int i=1;i<5;++i) {//bug à corriger : checker bords du jeu
			if(!map.collisions[indY][indX-i]) {
				players[p_index].x-=rect.width;
				break;
			}
			if(!map.collisions[indY][indX+i]) {
				players[p_index].x+=rect.width;
				break;
			}
			if(!map.collisions[indY-i][indX]) {
				players[p_index].y+=rect.height;
				break;
			}
			if(!map.collisions[indY+i][indX]) {
				players[p_index].y-=rect.height;
				break;
			}
		}
		indY=(players[p_index].y-OFFSET)/TILE_DIM,indX=players[p_index].x/TILE_DIM;
		players[p_index].sprite.setPosition(players[p_index].x,players[p_index].y);
		if(!map.collisions[indY+1][indX]) {
				players[p_index].vy=0;
				players[p_index].state=2;
				players[p_index].y+=players[p_index].speed*deltaTime;
		}
	}
}

bool GameLogic::updateCoin(int p_index,float deltaTime) {
	if(players[p_index].coin.collected) {
		players[p_index].coin.timer+=deltaTime;
		if(players[p_index].coin.timer>COIN_BLOCK_TIMER) {
			Modif m={(int)players[p_index].coin.x,(int)players[p_index].coin.y,COIN};
			modifs.push_back(m);
			map.map[players[p_index].coin.y][players[p_index].coin.x]=COIN;
			players[p_index].coin.timer=0;
			players[p_index].coin.collected=false;
			int x,y,other=(p_index+1)%2;
			size_t size=players[other].coinBlocks.size();
			for(size_t i=0;i<size;i++) {
				x=players[other].coinBlocks[i].x;
				y=players[other].coinBlocks[i].y;
				map.map[x][y]=COIN_BLOCK;
				map.collisions[x][y]=true;
				m={y,x,COIN_BLOCK};
				modifs.push_back(m);
			}
			return true;
		}
	}
	return false;
}

bool GameLogic::updateTimerBlocks(float deltaTime) {
	timer+=deltaTime;
	if(timer>TIMER_BLOCK_TIMER) {
		timer=0;
		size_t size=timerBlocks.size();
		int x,y,blockValue=(timerBlocksDisplayed)? EMPTY:TIMER_BLOCK;
		timerBlocksDisplayed=!timerBlocksDisplayed;
		for(size_t i=0;i<size;i++) {
			x=timerBlocks[i].x;
			y=timerBlocks[i].y;
			map.map[x][y]=blockValue;
			map.collisions[x][y]=(blockValue==TIMER_BLOCK);
			Modif m={y,x,blockValue};
			modifs.push_back(m);
		}
		return true;
	}
	return false;
}

void GameLogic::update(float deltaTime,vector<std::vector<sf::Sprite>> &gmap) {
	modifs.clear();

	handleEvents(deltaTime);
	handleCollisions(0,gmap,deltaTime);
	handleCollisions(1,gmap,deltaTime);

	interactEvent = false;
	bool reappear1=false,reappear2=false;
	if(updateCoin(0,deltaTime)) reappear2=true;
	if(updateCoin(1,deltaTime)) reappear1=true;
	if(updateTimerBlocks(deltaTime)) reappear1=reappear2=true;
	if(reappear1) handleCollisions2(0,gmap,deltaTime);
	if(reappear2) handleCollisions2(1,gmap,deltaTime);

	players[0].animate(deltaTime);
	players[1].animate(deltaTime);

	players[0].sprite.setPosition(players[0].x,players[0].y);
	players[1].sprite.setPosition(players[1].x,players[1].y);
}

GameLogic::~GameLogic(){}
