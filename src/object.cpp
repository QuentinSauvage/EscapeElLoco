#include "object.hpp"

Door::Door() : Entity(){}
Door::~Door(){}

Rope::Rope() : Entity(){}
Rope::~Rope(){}

Coin::Coin() : Entity(),timer(0),collected(false){}
Coin::Coin(float x,float y,float s,float r) : Entity(x,y,s,r),timer(0),collected(false) {}
Coin::~Coin(){}

Key::Key() : Entity(){}
Key::Key(float x,float y,float s,float r) : Entity(x,y,s,r),collected(false) {}
Key::~Key(){}

Chest::Chest(){}
Chest::~Chest(){}