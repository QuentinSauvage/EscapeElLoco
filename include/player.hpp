#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object.hpp"

#define DEFAULT_MAX_LIFE 3
#define DEFAULT_SPEED 300.0f

class Player : public Object {
	public:
		int life;
		int maxLife;
		float speed;

		Player();
		void init(float x, float y);
		void move(int direction, float deltaTime);

		bool isGrounded();
		void jump();

		~Player();
};

#endif