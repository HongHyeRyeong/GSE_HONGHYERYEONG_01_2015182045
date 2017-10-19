#include "stdafx.h"
#include "Object.h"

void Object::Update()
{
	float time = 0.5;

	if (x >= 250 || x <= -250)
		vX = -vX;
	else if (y >= 250 || y <= -250)
		vY = -vY;

	x += vX*time;
	y += vY*time;
}