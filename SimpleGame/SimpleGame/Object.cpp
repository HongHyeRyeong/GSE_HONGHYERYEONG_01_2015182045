#include "stdafx.h"
#include "Object.h"
#include "SceneMgr.h"

Object::Object(float x, float y, float size, int type) :x(x), y(y), size(size), type(type)
{
	if (type == OBJECT_BUILDING) {
		life = 500;
		vX = 0;
		vY = 0;

		rgb[0] = 1;
		rgb[1] = 1;
		rgb[2] = 0;
	}
	else if (type == OBJECT_CHARACTER) {
		life = 10;
		vX = 300.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);
		vY = 300.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);

		rgb[0] = 1;
		rgb[1] = 1;
		rgb[2] = 1;

		arrowTime = 0;
	}
	else if (type == OBJECT_BULLET) {
		life = 20;
		vX = 600.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);
		vY = 600.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);

		rgb[0] = 1;
		rgb[1] = 0;
		rgb[2] = 0;
	}
	else if (type == OBJECT_ARROW) {
		life = 10;
		vX = 100.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);
		vY = 100.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);

		rgb[0] = 0;
		rgb[1] = 1;
		rgb[2] = 0;

		arrowID = NULL;
	}
}

void Object::Update(float elapsedTime)
{
	float time = elapsedTime / 1000;

	if (x >= 250 || x <= -250)
		vX = -vX;
	else if (y >= 250 || y <= -250)
		vY = -vY;

	x += vX*time;
	y += vY*time;

	//lifeTime -= elapsedTime;

	if (type == OBJECT_CHARACTER)
		arrowTime += time;
}