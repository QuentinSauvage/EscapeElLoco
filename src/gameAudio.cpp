#include <SFML/Audio.hpp>
#include "gameAudio.hpp"
#include "iostream"

using namespace sf;

GameAudio::GameAudio() : playing(true) {}

void GameAudio::load() {
    if(!music.openFromFile(MUSIC_PATH)) std::cerr << LOAD_AUDIO_ERROR << MUSIC_PATH << std::endl;
    music.play();
    music.setLoop(true);
    music.setVolume(VOLUME);
    if(!jumpAudio.loadFromFile(JUMP_AUDIO_PATH)) std::cerr << LOAD_AUDIO_ERROR << JUMP_AUDIO_PATH << std::endl;
	else jumpSoundLeft.setBuffer(jumpAudio),jumpSoundRight.setBuffer(jumpAudio);
    jumpSoundLeft.setPosition(-1.0f,0.0f,0.0f);
    jumpSoundRight.setPosition(1.0f,0.0f,0.0f);
    if(!timerAudio.loadFromFile(TIMER_PATH)) std::cerr << LOAD_AUDIO_ERROR << TIMER_PATH << std::endl;
    else {
        timer.setBuffer(timerAudio);
        timer.setVolume(200);
    }
    if(!chestAudio.loadFromFile(CHEST_AUDIO_PATH)) std::cerr << LOAD_AUDIO_ERROR << CHEST_AUDIO_PATH << std::endl;
    else chest.setBuffer(chestAudio);
}

void GameAudio::playSound(sf::Sound &sound) {
    if(playing&&sound.getStatus()!=Sound::Playing)
        sound.play();
}

void GameAudio::changeState() {
    playing=!playing;
    if(!playing) music.stop();
    else music.play();
}

GameAudio::~GameAudio() {}