#include "stdafx.h"
#include "Object.h"

Object::Object()
{
}


Object::Object(float x, float y, float z, float size, float r, float g, float b, float a)
	:x(x), y(y), z(z), size(size), r(r), g(g), b(b), a(a)
{
}

Object::~Object()
{
}
