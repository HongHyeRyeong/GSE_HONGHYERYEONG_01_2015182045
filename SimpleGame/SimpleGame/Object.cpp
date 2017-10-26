#include "stdafx.h"
#include "Object.h"

Object::Object(float x, float y, float z, float size) :x(x), y(y), z(z), size(size)
{
	vX = 200.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);
	vY = 200.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);

	rgb[0] = 1;
	rgb[1] = 1;
	rgb[2] = 1;
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
}