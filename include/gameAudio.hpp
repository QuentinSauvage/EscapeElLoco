#ifndef _GAME_AUDIO_H_
#define _GAME_AUDIO_H_

#include <SFML/Audio.hpp>

#define LOAD_MUSIC_ERROR "Error when loading music"
#define LOAD_TIMER_ERROR "Error when loading timer sound"
#define LOAD_JUMP_ERROR "Error when loading jump sound"
#define JUMP_AUDIO_PATH "audio/jump.wav"
#define MUSIC_PATH "audio/music.wav"
#define TIMER_PATH "audio/timer.wav"
#define VOLUME 20

class GameAudio {
	public:
        sf::Music music;
        sf::SoundBuffer jumpAudio, timerAudio;
		sf::Sound jumpSoundLeft,jumpSoundRight, timer;
        bool playing;

		GameAudio();
		void load();
        void changeState();
		void playSound(sf::Sound &sound);
		~GameAudio();
};

#endif