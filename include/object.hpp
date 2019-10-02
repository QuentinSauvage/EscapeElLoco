#ifndef _OBJECT_H_
#define _OBJECT_H_

class Object {
	public:
		float x,y,scale,rotation;

		Object();
		Object(float x, float y, float scale, float rotation);
		~Object();
};

#endif