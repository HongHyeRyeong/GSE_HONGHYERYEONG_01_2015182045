#pragma once
#include <cstdlib>

class Object
{
	int type, id;
	float x, y;
	float size;
	float rgb[3];
	float vX, vY;
	float life, lifeTime;
	float attackTime;
	float texTime;
	int m_texNum[2];

public:
	Object() {};
	Object(float x, float y, float size, int type, int id);
	~Object() {};

	void Update(float elapsedTime);
	void UpdateTex();
	void Collision(int num) { life -= num; }

	float getX() { return x; };
	float getY() { return y; };
	float getSize() { return size; };
	float getRGB(int i) { return rgb[i]; }
	float getLife() { return life; }
	float gerLifeTime() { return lifeTime; }
	int getId() { return id; }
	float getAttackTime() { return attackTime; }
	int getTexNum(int i) { return  m_texNum[i]; }

	void setAttackTime(float time) { attackTime = time; }
};

