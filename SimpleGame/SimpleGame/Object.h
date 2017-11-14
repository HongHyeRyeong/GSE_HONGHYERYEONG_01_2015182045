#pragma once
#include <cstdlib>

class Object
{
	int type;
	float x, y;
	float size;
	float rgb[3];
	float vX, vY;
	float life, lifeTime;

	int arrowID;
	float arrowTime;

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
	int getArrowID() { return arrowID; }
	float getArrowTime() { return arrowTime; }

	void setRGB(int i, float j) { rgb[i] = j; }
	void setArrowID(int i) { arrowID = i; }
	void setArrowTime(float time) { arrowTime = time; }
};

