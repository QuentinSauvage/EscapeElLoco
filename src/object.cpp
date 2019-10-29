#include "object.hpp"

Coin::Coin() : Entity(),timer(0),collected(false){}
Coin::Coin(float x,float y,float s,float r,bool b) : Entity(x,y,s,r),timer(0),collected(b) {}
Coin::~Coin(){}

Key::Key() : Entity(){}
Key::Key(float x,float y,float s,float r) : Entity(x,y,s,r),collected(false) {}
Key::~Key(){}