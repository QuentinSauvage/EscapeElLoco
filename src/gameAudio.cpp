#include <SFML/Audio.hpp>
#include "gameAudio.hpp"
#include "iostream"

using namespace sf;

GameAudio::GameAudio() : playing(true) {}

void GameAudio::load() {
    if(!music.openFromFile(MUSIC_PATH))  std::cerr << LOAD_MUSIC_ERROR << std::endl;
    music.play();
    music.setLoop(true);
    music.setVolume(20);
    if(!jump_audio.loadFromFile(JUMP_AUDIO_PATH)) std::cerr << LOAD_JUMP_ERROR << std::endl;
	else jump_sound.setBuffer(jump_audio);
    jump_sound.setPosition(-1.0f,0.0f,0.0f);
}

void GameAudio::changeState() {
    playing = !playing;
    if(!playing)
        music.stop();
    else
        music.play();
}

GameAudio::~GameAudio() {}