#include "stdafx.h"
#include "Object.h"
#include "SceneMgr.h"

Object::Object(float x, float y, int type, int id) :x(x), y(y), type(type), id(id), attackTime(0)
{
	if (type == OBJECT_BUILDING) {
		life = 500;
		vX = 0;
		vY = 0;

		size = 100;

		rgb[0] = 1;
		rgb[1] = 1;
		rgb[2] = 1;
	}
	else if (type == OBJECT_CHARACTER) {
		life = 100;
		vX = 300.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);
		vY = 300.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);

		size = 30;

		rgb[0] = 1;
		rgb[1] = 1;
		rgb[2] = 1;
	}
	else if (type == OBJECT_BULLET) {
		life = 15;
		vX = 600.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);
		vY = 600.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);

		size = 4;

		if (id == 1) {
			rgb[0] = 1;
			rgb[1] = 0;
			rgb[2] = 0;
		}
		else if (id == 2) {
			rgb[0] = 0;
			rgb[1] = 0;
			rgb[2] = 1;
		}
	}
	else if (type == OBJECT_ARROW) {
		life = 10;
		vX = 100.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);
		vY = 100.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);

		size = 4;

		if (id == 1) {
			rgb[0] = 0.5;
			rgb[1] = 0.2;
			rgb[2] = 0.7;
		}
		else if (id == 2) {
			rgb[0] = 1;
			rgb[1] = 1;
			rgb[2] = 0;
		}
	}
}

void Object::Update(float elapsedTime)
{
	float time = elapsedTime / 1000;

	if (type == OBJECT_CHARACTER) {
		if (x >= fieldW / 2 || x <= -fieldW / 2)
			vX = -vX;
		else if (y >= fieldH / 2 || y <= -fieldH / 2)
			vY = -vY;
	}
	else if (type == OBJECT_BULLET || type == OBJECT_ARROW) {
		if (x >= fieldW / 2 || x <= -fieldW / 2 || y >= fieldH / 2 || y <= -fieldH / 2)
			life = 0;
	}

	x += vX*time;
	y += vY*time;

	//lifeTime -= elapsedTime;
	attackTime += time;
}