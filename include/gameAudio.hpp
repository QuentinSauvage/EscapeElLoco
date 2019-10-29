#ifndef _GAME_AUDIO_H_
#define _GAME_AUDIO_H_

#include <SFML/Audio.hpp>

#define LOAD_AUDIO_ERROR "Error when loading: "
#define JUMP_AUDIO_PATH "audio/jump.wav"
#define MUSIC_PATH "audio/music.wav"
#define TIMER_PATH "audio/timer.wav"
#define CHEST_AUDIO_PATH "audio/chest.wav"
#define VOLUME 20

class GameAudio {
	public:
        sf::Music music;
        sf::SoundBuffer jumpAudio,timerAudio,chestAudio;
		sf::Sound jumpSoundLeft,jumpSoundRight,timer,chest;
        bool playing;

		GameAudio();
		void load();
        void changeState();
		void playSound(sf::Sound &sound);
		~GameAudio();
};

#endif