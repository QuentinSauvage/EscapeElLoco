#include <fstream>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "gameLogic.hpp"

using namespace std;

GameLogic::GameLogic(GameAudio &audio) : gameAudio(audio),level(1),p_index(0),pause(false),end(false),cheating(false) {
	initLevel();
}

void GameLogic::clear() {
	++level;
	end=true;
	modifs.clear();
	map.map.clear();
	map.collisions.clear();
	timerBlocks.clear();
	collapseBlocks[0].clear();
	collapseBlocks[1].clear();
	collapsingBlocks.clear();
	gameAudio.timer.stop();
	players[0].doorOpened=false;
	players[0].coinBlocks.clear();
	players[1].coinBlocks.clear();
	players[1].doorOpened=false;
}

void GameLogic::initLevel() {
	if(level>2) {
		gameAudio.timer.stop();
		gameAudio.music.stop();
		return;
	}
	timer=timerElLoco=0;
	godMode=hardcoreMode=interactEvent=timerBlocksDisplayed=elLocoDisplayed=false;
	extractMap();
}

void GameLogic::addModif(int indX,int indY,int value) {
	map.map[indY][indX]=value;
	modifs.push_back({indX,indY,value,16,16});
}

int GameLogic::tileType(int tile) const {
	if(tile==EMPTY||tile==COIN||tile==CHAIR_L||tile==CHAIR_R||tile==BOUQUET||
	tile==DOOR_BC||tile==DOOR_TC||tile==DOOR_BO||tile==DOOR_TO||
	tile==KEY||tile==CHEST_LC||tile==CHEST_RC||tile==CHEST_LO||tile==CHEST_RO) return -1;
	return tile!=LADDER&&tile!=ROPE&&tile!=ROPE_TOP&&tile!=ROPE_END;
}

void GameLogic::interact(int indX, int indY,float deltaTime) {
	if(map.map[indY][indX]==COIN&&!players[p_index].coin.collected) {
		Coin &coin=players[p_index].coin;
		if(players[p_index].coin.timer>0) {
			addModif(coin.y,coin.x,EMPTY);
			coin.timer=0;
		}
		if(gameAudio.playing) gameAudio.timer.play();
		coin = Coin(indX,indY,1,0,true);
		addModif(indX,indY,EMPTY);
		int x,y,other=(p_index+1)%2;
		size_t size=players[other].coinBlocks.size();
		for(size_t i=0;i<size;i++) {
			x=players[other].coinBlocks[i].x;
			y=players[other].coinBlocks[i].y;
			addModif(y,x,EMPTY);
			map.collisions[x][y]=-1;
		}
	} else if(map.map[indY][indX]==LADDER||map.map[indY+1][indX]==LADDER||map.map[indY][indX]==ROPE||
			map.map[indY+1][indX]==ROPE||map.map[indY][indX]==ROPE_END||map.map[indY+1][indX]==ROPE_END) {
		float tmp=players[p_index].y;
		players[p_index].climb(climbing,deltaTime);
		int y=(players[p_index].y)/TILE_DIM;
		if(map.collisions[y][indX]>0) players[p_index].y=tmp;
	} else if(map.map[indY][indX]==KEY) {
		Key &key=players[p_index].key;
		key.collected=true;
		addModif(key.y,key.x,EMPTY);
	} else if((map.map[indY][indX]==CHEST_LC||map.map[indY][indX]==CHEST_RC)) {
		Key &key=players[p_index].key;
		addModif(key.y,key.x,KEY);
		if(map.map[indY][indX]==CHEST_LC) {
			addModif(indX,indY,CHEST_LO);
			if(map.map[indY][indX+1]==CHEST_RC)
				addModif(indX+1,indY,CHEST_RO);
		} else {
			if(map.map[indY][indX-1]==CHEST_LC)
			addModif(indX-1,indY,CHEST_LO);
			addModif(indX,indY,CHEST_RO);
		}
		gameAudio.playSound(gameAudio.chest);
	} else if((map.map[indY][indX]==DOOR_TC||map.map[indY][indX]==DOOR_BC)&&players[p_index].key.collected) {
		players[p_index].doorOpened=true;
		if(map.map[indY][indX]==DOOR_TC) {
			addModif(indX,indY,DOOR_TO);
			addModif(indX,indY+1,DOOR_BO);
		} else {
			addModif(indX,indY,DOOR_BO);
			addModif(indX,indY-1,DOOR_TO);
		}
		if(players[(p_index+1)%2].doorOpened) {
			clear();
			initLevel();
		}
	}
}

void GameLogic::extractMap() {
	ifstream levelMap;
	string filename=PATH_LEVEL;
	char c;
	int tmp;
	filename+=to_string(level)+CSV;
	levelMap.open(filename.c_str());

	vector<CoinBlock> coinBlocks;
	if(levelMap.good()) {
		int posX1,posX2,posY1,posY2;
		levelMap >> map.height >> c >> map.width >> c;
		levelMap >> posX1 >> c >> posY1 >> c;
		levelMap >> posX2 >> c >> posY2 >> c;
		int sep=map.width>>1;
		if(level==2) sep++;//level2 doesn't divide the map in two equal parts
		map.map.emplace_back();
		map.collisions.emplace_back();
		for(int i=0;i<map.height;i++) {
			for(int j=0;j<map.width;j++) {
				levelMap >> tmp >> c;
				if(tmp==COIN_BLOCK) {
					if(j>sep) players[1].coinBlocks.push_back(CoinBlock(i,j,1,0));
					else players[0].coinBlocks.push_back(CoinBlock(i,j,1,0));
				} else if(tmp==COIN) {
					if(j>sep) players[1].coins.push_back(Coin());
					else players[0].coins.push_back(Coin());
				} else if(tmp==KEY) {
					if(j>sep) players[1].key=Key(i,j,1,0);
					else players[0].key=Key(i,j,1,0);
					tmp=EMPTY;
				} else if(tmp==TIMER_BLOCK) {
					timerBlocks.push_back(TimerBlock(i,j,0,true,1,0));
					tmp=EMPTY;
				} else if(tmp==COLLAPSE_BLOCK) {
					if(j>sep) collapseBlocks[1].push_back(CollapseBlock(i,j,0,1,0));
					else collapseBlocks[0].push_back(CollapseBlock(i,j,0,1,0));
				}
				map.map.back().push_back(tmp);
				map.collisions.back().push_back(tileType(tmp));
			}
			map.map.emplace_back();
			map.collisions.emplace_back();
		}
		levelMap.close();
		players[0].init(posX1,posY1,PLAYER3_SPRITE);
		players[1].init(posX2,posY2,PLAYER4_SPRITE);
	} else cerr << LOAD_MAP_ERROR,exit(1);
}

//handle inputs from the user
void GameLogic::handleEvents(float deltaTime,int keyCode) {
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
			if(godMode||players[0].state!=2) {
				players[0].vy=JUMP_VELOCITY;
				players[0].state=2;
				gameAudio.playSound(gameAudio.jumpSoundLeft);
			}
			if(godMode||players[1].state!=2) {
				players[1].vy=JUMP_VELOCITY;
				players[1].state=2;
				gameAudio.playSound(gameAudio.jumpSoundRight);
			}
		}
		if(keyCode==sf::Keyboard::R) {
			players[0].x=players[0].originX;
			players[1].x=players[1].originX;
			players[0].y=players[0].originY;
			players[1].y=players[1].originY;
			players[0].state=players[1].state=0;
		}
		if(keyCode==sf::Keyboard::A) gameAudio.changeState();
		if(keyCode==sf::Keyboard::G) {
			if(!cheating) {
				cheating=true;
				cout << CHEAT_MESSAGE << endl;
			}
			godMode=!godMode;
		}
		if(keyCode==sf::Keyboard::H) hardcoreMode=!hardcoreMode;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			climbing=true;
			interactEvent = true;
		} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			climbing=false;
			interactEvent = true;
		}
	}
}

void GameLogic::jump(float deltaTime) {
	if(players[p_index].state==2) {
    	players[p_index].vy-=(GRAVITY*deltaTime);
		players[p_index].y-=players[p_index].vy*deltaTime;
		players[p_index].sprite.setPosition(players[p_index].x,players[p_index].y);
  	}
}

void GameLogic::initBox(Box &b) const {
	if(b.flags&1)			b.centerX=(players[p_index].x)/TILE_DIM;
	if((b.flags&(2))>1)		b.centerY=(players[p_index].y)/TILE_DIM;
	if((b.flags&(4))>>2)	b.top=(players[p_index].y-PLAYER_DIM_SCALED)/TILE_DIM;
	if((b.flags&(8))>>3)	b.down=(players[p_index].y+PLAYER_DIM_SCALED)/TILE_DIM;
	if((b.flags&(16))>>4)	b.left=(players[p_index].x-PLAYER_DIM_SCALED)/TILE_DIM;
	if((b.flags&(32))>>5)	b.right=(players[p_index].x+PLAYER_DIM_SCALED)/TILE_DIM;
}

void GameLogic::handleCollisions(const vector<vector<sf::Sprite>> &gmap,float deltaTime) {
	Box b;
	b.flags=63;
	initBox(b);
	sf::FloatRect rect,r=players[p_index].sprite.getGlobalBounds();

	if(interactEvent) interact(b.centerX,b.centerY,deltaTime);
	jump(deltaTime);

	int limY=players[p_index].vy;
	if(limY<0) limY*=-1;
	limY=(limY>JUMP_VELOCITY)?limY/JUMP_VELOCITY:0;
	for(int i=0;i<=limY+1;++i) {
		//collision top
		if(i&&map.collisions[b.top-i][b.centerX]>0 && (players[p_index].sprite.getGlobalBounds().intersects(gmap[b.top-i][b.centerX].getGlobalBounds(),rect))) {
			if(map.map[b.top-i][b.centerX]!=COLLAPSE_BLOCK) {
				players[p_index].y+=rect.height;
				players[p_index].vy=0;
				players[p_index].sprite.setPosition(players[p_index].x,players[p_index].y);
				r = players[p_index].sprite.getGlobalBounds();
				b.flags=61;
				initBox(b);
				break;
			}
		}

		//collision down
		if(map.collisions[b.down+i][b.centerX]>0 && (players[p_index].sprite.getGlobalBounds().intersects(gmap[b.down+i][b.centerX].getGlobalBounds(),rect))) {
			players[p_index].y-=rect.height;
			players[p_index].state=(players[p_index].vx!=0)? 1 : 0;
			players[p_index].spriteRect.left=IDLE_1_OFFSET;
			players[p_index].vy=0;
			players[p_index].sprite.setPosition(players[p_index].x,players[p_index].y);
			r = players[p_index].sprite.getGlobalBounds();
			b.flags=61;
			initBox(b);
			break;
		}
	}

	int limX=-players[p_index].oldX;
	limX=(limX>16)?limX/16:0;
	for(int i=0;i<=limX;++i) {
		//collision left
		if(map.collisions[b.centerY][b.left-i]>0 && (players[p_index].sprite.getGlobalBounds().intersects(gmap[b.centerY][b.left-i].getGlobalBounds(),rect))) {
			if(map.map[b.centerY][b.left]!=COLLAPSE_BLOCK) {
				players[p_index].x+=rect.width;
				players[p_index].sprite.setPosition(players[p_index].x,players[p_index].y);
				b.flags=61;
				initBox(b);
				break;
			}
		}
		//collision right
		if(map.collisions[b.centerY][b.right+i]>0 && (players[p_index].sprite.getGlobalBounds().intersects(gmap[b.centerY][b.right+i].getGlobalBounds(),rect))) {
			if(map.map[b.centerY][b.right]!=COLLAPSE_BLOCK) {
				players[p_index].x-=rect.width;
				players[p_index].sprite.setPosition(players[p_index].x,players[p_index].y);
				b.flags=9;
				initBox(b);
				break;
			}
		}
	}

	//check if the player is falling
	if(players[p_index].state!=2&&map.collisions[b.down][b.centerX]==-1) {
		players[p_index].state=2;
		players[p_index].vy=0;
	} else if(!map.collisions[b.down][b.centerX])
		players[p_index].vy=1;

	if(map.map[b.down][b.centerX]==COLLAPSE_BLOCK) {
		for(CollapseBlock cb : collapseBlocks[p_index])
			if(cb.x==b.down&&cb.y==b.centerX)
				collapsingBlocks.push_back(cb);
	}
}

void GameLogic::handleCollisions2(const vector<vector<sf::Sprite>> &gmap, float deltaTime) {
	Box b;
	b.flags=63;
	initBox(b);
	sf::FloatRect rect;
	int i=1;

	if(map.collisions[b.centerY][b.centerX]>0 && players[p_index].sprite.getGlobalBounds().intersects(gmap[b.centerY][b.centerX].getGlobalBounds(),rect)) {
		bool move=false;
		while(true) {
			//search for the closest empty tile (doesn't check every tile)
			if(b.left-i>=0&&map.collisions[b.centerY][b.left-i]<=0) {
				players[p_index].x-=(rect.width+(i-1)*TILE_DIM+PLAYER_DIM_SCALED);
				move=true;
			} else if(b.right+i<map.width&&map.collisions[b.centerY][b.right+i]<=0) {
				players[p_index].x+=(rect.width+(i-1)*TILE_DIM-PLAYER_DIM_SCALED);
				move=true;
			} else if(b.top-i>=0&&map.collisions[b.top-i][b.centerX]<=0) {
				players[p_index].y+=(rect.height+(i-1)*TILE_DIM-PLAYER_DIM_SCALED);
				move=true;
			} else if(b.down+i<map.height&&map.collisions[b.down+i][b.centerX]<=0) {
				players[p_index].y-=(rect.height+(i-1)*TILE_DIM+PLAYER_DIM_SCALED);
				move=true;
			} else if(b.left-i>=0&&b.top-i>=0&&map.collisions[b.top-i][b.left-i]<=0) {
				players[p_index].x-=(rect.width+(i-1)*TILE_DIM+PLAYER_DIM_SCALED);
				players[p_index].y-=(rect.height+(i-1)*TILE_DIM+PLAYER_DIM_SCALED);
				move=true;
			} else if(b.right+i<map.width&&b.down+i<map.height&&map.collisions[b.down+i][b.left+i]<=0) {
				players[p_index].x+=(rect.width+(i-1)*TILE_DIM-PLAYER_DIM_SCALED);
				players[p_index].y+=(rect.height+(i-1)*TILE_DIM-PLAYER_DIM_SCALED);
				move=true;
			} else if(b.right+i<map.width&&b.top-i>=0&&map.collisions[b.top-i][b.right+i]<=0) {
				players[p_index].x+=(rect.width+(i-1)*TILE_DIM-PLAYER_DIM_SCALED);
				players[p_index].y-=(rect.height+(i-1)*TILE_DIM+PLAYER_DIM_SCALED);
				move=true;
			} else if(b.left-i>=0&&b.down+i<map.height&&map.collisions[b.down+i][b.left-i]<=0) {
				players[p_index].x-=(rect.width+(i-1)*TILE_DIM+PLAYER_DIM_SCALED);
				players[p_index].y+=(rect.height+(i-1)*TILE_DIM-PLAYER_DIM_SCALED);
				move=true;
			}
			if(move)
				break;
			++i;
		}
	}
}

bool GameLogic::updateCoin(float deltaTime) {
	Coin &coin=players[p_index].coin;
	if(coin.collected) {
		coin.timer+=deltaTime;
		if(coin.timer>COIN_BLOCK_TIMER) {
			addModif(coin.x,coin.y,COIN);
			coin.timer=0;
			coin.collected=false;
			int x,y,other=(p_index+1)%2;
			size_t size=players[other].coinBlocks.size();
			for(size_t i=0;i<size;i++) {
				x=players[other].coinBlocks[i].x;
				y=players[other].coinBlocks[i].y;
				map.collisions[x][y]=1;
				addModif(y,x,COIN_BLOCK);
			}
			return true;
		}
	}
	return false;
}

bool GameLogic::updateCollapseBlocks(float deltaTime) {
	for(vector<CollapseBlock>::iterator it=collapsingBlocks.begin();it!=collapsingBlocks.end();++it) {
		it->timer+=deltaTime;
		if(it->displayed&&it->timer>COLLAPSE_BLOCK_TIMER_1) {
			it->displayed=false;
			it->timer=0;
			addModif(it->y,it->x,EMPTY);
			map.collisions[it->x][it->y]=-1;
		} else if(!it->displayed&&it->timer>COLLAPSE_BLOCK_TIMER_2) {
			it->timer=0;
			addModif(it->y,it->x,COLLAPSE_BLOCK);
			map.collisions[it->x][it->y]=1;
			collapsingBlocks.erase(it);
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
			addModif(y,x,blockValue);
			map.collisions[x][y]=(blockValue==TIMER_BLOCK)? 1:-1;
		}
		return true;
	}
	return false;
}

void GameLogic::animateElLoco(float deltaTime) {
	timerElLoco+=deltaTime;
	if(timerElLoco>TIMER_EL_LOCO_2) {
		timerElLoco=0;
		elLocoDisplayed=false;
		players[0].sprite=players[0].oldSprite;
		players[1].sprite=players[1].oldSprite;
	} else if(timerElLoco>TIMER_EL_LOCO_1&&!elLocoDisplayed) {
		elLocoDisplayed=true;
		players[0].oldSprite=players[0].sprite;
		players[1].oldSprite=players[1].sprite;
		players[0].sprite=players[1].sprite=elLoco;
	} else if(!elLocoDisplayed){
		players[0].animate(deltaTime);
		players[1].animate(deltaTime);
	}
}

void GameLogic::update(float deltaTime,const vector<std::vector<sf::Sprite>> &gmap,int keyCode) {
	bool reappear1=false,reappear2=false;
	modifs.clear();

	interactEvent = false;
	handleEvents(deltaTime,keyCode);
	if(updateCollapseBlocks(deltaTime)) reappear1=reappear2=true;
	if(updateTimerBlocks(deltaTime)) reappear1=reappear2=true;

	if(updateCoin(deltaTime)) reappear1=true;
	if(reappear1) handleCollisions2(gmap,deltaTime);
	handleCollisions(gmap,deltaTime);
	p_index=1;
	if(updateCoin(deltaTime)) reappear2=true;
	if(reappear2) handleCollisions2(gmap,deltaTime);
	handleCollisions(gmap,deltaTime);
	p_index=0;

	animateElLoco(deltaTime);

	players[0].sprite.setPosition(players[0].x,players[0].y);
	players[1].sprite.setPosition(players[1].x,players[1].y);
}

GameLogic::~GameLogic(){}
