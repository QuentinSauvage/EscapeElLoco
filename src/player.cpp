#include "player.hpp"

Player::Player() : Object(), life(DEFAULT_MAX_LIFE), maxLife(DEFAULT_MAX_LIFE), speed(DEFAULT_SPEED) {} 

void Player::init(float x, float y){
	this->x = x;
	this->y = y;
}

bool Player::isGrounded() {
	return false;
}

void Player::move(int direction, float deltaTime) {
	x += direction * speed * deltaTime; 
}

void Player::jump() {

}

Player::~Player() {}