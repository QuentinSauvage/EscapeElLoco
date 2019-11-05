#include "gameGraphics.hpp"
#include <math.h>
#include <iostream>

using namespace sf;

GameGraphics::GameGraphics(GameLogic &gl) : hardcoreMode(false),end(false),gameLogic(gl),rotation(0) {
	init();
}

void GameGraphics::loadMap() {
    const char *sprite1, *sprite2;
    if(!(gameLogic.level%2)) sprite1=PLAYER1_SPRITE,sprite2=PLAYER2_SPRITE;
    else sprite1=PLAYER3_SPRITE,sprite2=PLAYER4_SPRITE;
    if(!playersTexture[0].loadFromFile(sprite1)) {
        std::cerr << LOAD_GRAPHICS_ERROR << sprite1 << std::endl;
        exit(1);
    }
    if(!playersTexture[1].loadFromFile(sprite2)) {
        std::cerr << LOAD_GRAPHICS_ERROR << sprite2 << std::endl;
        exit(1);
    }

    sf::Sprite s;
    map.emplace_back();
    for(int i=0; i<gameLogic.map.height;i++) {
        for(int j=0; j<gameLogic.map.width;j++) {
            s.setTexture(background);
            int tileNumber=gameLogic.map.map[i][j];
            if(tileNumber==TABLE) {
                s.setTextureRect(IntRect((tileNumber%8)<<4,((tileNumber>>3)<<4)+8,16,8));
                s.setPosition(j<<6,(i<<6)+32);
            } else {
                if(tileNumber==COLLAPSE_BLOCK)
                    s.setTextureRect(IntRect((tileNumber%8)<<4,(tileNumber>>3)<<4,16,8));
                else
                    s.setTextureRect(IntRect((tileNumber%8)<<4,(tileNumber>>3)<<4,16,16));
                s.setPosition(j<<6,i<<6);
            }
            s.setScale(4.f,4.f);
            map.back().push_back(s);
        }
        map.emplace_back();
    }
    for(int i=0;i<2;i++) gameLogic.players[i].setSprite(playersTexture[i]);
    
    //init bounds for the views
    Vector2f size=viewLeft.getSize();
    int widthLimit=(gameLogic.map.width>>1)<<6;
    bound_min_y=size.y/2;
    bound_max_y=gameLogic.map.height*64-bound_min_y;
    left_center_x=size.x/2;
    right_center_x=left_center_x+widthLimit;
    bound_x1=widthLimit-left_center_x;
    bound_x2=gameLogic.map.width*64-left_center_x;
}

void GameGraphics::load() {
    if(!background.loadFromFile(LOAD_TILESET)) {
        std::cerr << LOAD_GRAPHICS_ERROR << LOAD_TILESET << std::endl;
        exit(1);
    }
    if(!font.loadFromFile(LOAD_FONT)) {
        std::cerr << LOAD_GRAPHICS_ERROR << LOAD_FONT << std::endl;
        return;
    }
    if(!elLocoTexture.loadFromFile(LOAD_EL_LOCO)) {
        std::cerr << LOAD_GRAPHICS_ERROR << LOAD_EL_LOCO << std::endl;
        exit(1);
    }
    elLocoSprite.setTexture(elLocoTexture);
    elLocoSprite.setOrigin(32,32);
    gameLogic.elLoco=elLocoSprite;
    loadMap();
}

void GameGraphics::checkUpdate() {
    if(gameLogic.end) {
        gameLogic.end=false;
        rotation=0;
        reinit();
    } else {
        GameLogic::Modif m;
        for(size_t i=0;i<gameLogic.modifs.size();i++) {
            m=gameLogic.modifs[i];
            map[m.y][m.x].setTextureRect(IntRect((m.value%8)<<4,(m.value>>3)<<4,m.w,m.h));
        }
    }
}

//Each view only draw their part of the screen
void GameGraphics::drawBackground(int k) {
    int y1,y2,x1,x2,limitX1,limitX2,sizeX,sizeY;
    Vector2f center;
    View view;

    if(k) {
        view=viewRight;
        limitX1=screenLimit;
        limitX2=gameLogic.map.width;
    } else {
        view=viewLeft;
        limitX1=0;
        limitX2=screenLimit;
    }

    center=view.getCenter();
    sizeY=view.getSize().x/2;
    sizeX=view.getSize().y/2;
    x1=(((int)(center.x-sizeX))>>6)-1;
    x2=(((int)(center.x+sizeX))>>6)+1;
    y1=(((int)(center.y-sizeY))>>6)-1;
    y2=(((int)(center.y+sizeY))>>6)+1;
    if(x1<limitX1) x1=limitX1;
    if(x2>limitX2) x2=limitX2;
    if(y1<0) y1=0;
    if(y2>gameLogic.map.height) y2=gameLogic.map.height;

    for(int i=y1;i<y2;i++)
        for(int j=x1;j<x2;j++) {
            if(gameLogic.map.map[i][j]!=EMPTY)
                window.draw(map[i][j]);
        }
}

void GameGraphics::buildWindow() {
    VideoMode screen=VideoMode::getDesktopMode();
    window.create(VideoMode(screen.width*0.75, screen.height*0.75),TITLE,Style::Titlebar|Style::Close);
    window.setPosition(Vector2i((screen.width>>1)-(window.getSize().x>>1),(screen.height>>1)-(window.getSize().y>>1)));
	window.setFramerateLimit(FRAMERATE);

    //default values
    viewLeft.setCenter(LEFT_CENTER_X,CENTER_Y);
    viewLeft.setViewport(FloatRect(0.f,0.f,0.498f,1.f));
    viewRight.setCenter(RIGHT_CENTER_X,CENTER_Y);
    viewRight.setViewport(FloatRect(0.501f,0.f,0.498f,1.f));

    float sizeX=(window.getSize().x*0.004f);
	border.setPosition((window.getSize().x/2)-(sizeX/2),0);
    border.setSize(sf::Vector2f(sizeX,window.getSize().y));
    border.setFillColor(Color(192,192,192));
}

void GameGraphics::reinit() {
    map.clear();
    loadMap();
    screenLimit=gameLogic.map.width>>1;
}

void GameGraphics::init() {
    buildWindow();
    load();

    //build framerateText
    framerateText.setFont(font);
    framerateText.setCharacterSize(FRAMERATE_FONT_SIZE);
    framerateText.setFillColor(Color::White);
    framerateText.setOutlineThickness(1);
    framerateText.setOutlineColor(Color::Blue);
    framerateText.setPosition(window.getSize().x-100,0);

    screenLimit=gameLogic.map.width>>1;
}

float GameGraphics::clamp(float v,float min,float max) const {
    if(v<min) return min;
    if(v>max) return max;
    return v; 
}

void GameGraphics::displayEndGame() {
    window.setView(window.getDefaultView());
    window.clear(Color::White);
    sf::Text endText;
    endText.setFont(font);
    endText.setCharacterSize(END_FONT_SIZE);
    endText.setFillColor(Color::Black);
    endText.setOutlineThickness(1);
    endText.setOutlineColor(Color::Yellow);
    endText.setPosition(100,100);
    endText.setString(END_TEXT); 
    window.draw(endText);

    sf::Text thanksText;
    thanksText.setFont(font);
    thanksText.setCharacterSize(THANKS_FONT_SIZE);
    thanksText.setFillColor(Color::Black);
    thanksText.setOutlineThickness(1);
    thanksText.setOutlineColor(Color::Yellow);
    thanksText.setPosition(window.getSize().x-500,window.getSize().y-100);
    thanksText.setString(THANKS_TEXT);
    window.draw(thanksText);

    elLocoSprite.setScale(4.0f,4.0f);
    elLocoSprite.setPosition(window.getSize().x>>1,window.getSize().y-300);
    window.draw(elLocoSprite);

    window.display();
}

void GameGraphics::update(float deltaTime) {
    if(gameLogic.level>2) {
        if(!end) {
            end=true;
            displayEndGame();
        }
        return;
    }
	framerate=1.f/(deltaTime);
	framerateText.setString(std::to_string((int)ceil(framerate))+" fps");    
    
    if(hardcoreMode!=gameLogic.hardcoreMode) {
        if(hardcoreMode) {
            rotation=0;
            viewLeft.setRotation(0);
            viewRight.setRotation(0);
        }
        hardcoreMode=!hardcoreMode;
    }
    if(hardcoreMode) {
        viewLeft.setRotation(-rotation);
        viewRight.setRotation(rotation);
        rotation+=ROTATION*deltaTime;
    }

    checkUpdate();

    //Draw
    viewLeft.setCenter(clamp(gameLogic.players[0].x,left_center_x,bound_x1),clamp(gameLogic.players[0].y,bound_min_y,bound_max_y));
    viewRight.setCenter(clamp(gameLogic.players[1].x,right_center_x,bound_x2),clamp(gameLogic.players[1].y,bound_min_y,bound_max_y));
    window.setView(window.getDefaultView());
    window.clear(Color::Black);    

    window.setView(viewLeft);
    drawBackground(0);
    window.draw(gameLogic.players[0].sprite);

    window.setView(viewRight);
    drawBackground(1);
    window.draw(gameLogic.players[1].sprite);
    
    window.setView(window.getDefaultView());
    window.draw(framerateText);
    window.draw(border);
    window.display();
}

GameGraphics::~GameGraphics() {}
