#include <SFML/Audio.hpp>
#include "gameAudio.hpp"
#include "iostream"

using namespace sf;

GameAudio::GameAudio() : playing(true) {}

void GameAudio::load() {
    if(!music.openFromFile(MUSIC_PATH)) std::cerr << LOAD_MUSIC_ERROR << std::endl;
    music.play();
    music.setLoop(true);
    music.setVolume(VOLUME);
    if(!jumpAudio.loadFromFile(JUMP_AUDIO_PATH)) std::cerr << LOAD_JUMP_ERROR << std::endl;
	else jumpSoundLeft.setBuffer(jumpAudio),jumpSoundRight.setBuffer(jumpAudio);
    jumpSoundLeft.setPosition(-1.0f,0.0f,0.0f);
    jumpSoundRight.setPosition(1.0f,0.0f,0.0f);
    if(!timerAudio.loadFromFile(TIMER_PATH)) std::cerr << LOAD_TIMER_ERROR << std::endl;
    else timer.setBuffer(timerAudio);
}

void GameAudio::changeState() {
    playing=!playing;
    if(!playing) music.stop();
    else music.play();
}

GameAudio::~GameAudio() {}