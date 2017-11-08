#pragma once
#include <cstdlib>

class Object
{
	float x, y;
	float size;
	float rgb[3];
	float vX, vY;
	float life, lifeTime;

public:
	Object() {};
	Object(float x, float y, float size, int type);
	~Object() {};

	void Update(float elapsedTime);
	void Collision(int num) { life -= num; }

	float getX() { return x; };
	float getY() { return y; };
	float getSize() { return size; };
	float getRGB(int i) { return rgb[i]; }
	float getLife() { return life; }
	float gerLifeTime() { return lifeTime; }

	void setRGB(int i, float j) { rgb[i] = j; }
};

