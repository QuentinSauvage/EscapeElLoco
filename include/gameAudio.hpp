#ifndef _GAME_AUDIO_H_
#define _GAME_AUDIO_H_

#include <SFML/Audio.hpp>

#define LOAD_MUSIC_ERROR "Error when loading music"
#define LOAD_JUMP_ERROR "Error when loading jump sound"
#define JUMP_AUDIO_PATH "audio/jump.wav"
#define MUSIC_PATH "audio/music.wav"
#define VOLUME 30

class GameAudio {
	public:
        sf::Music music;
        sf::SoundBuffer jumpAudio;
		sf::Sound jumpSoundLeft,jumpSoundRight;
        bool playing;

		GameAudio();
		void load();
        void changeState();
		~GameAudio();
};

#endif