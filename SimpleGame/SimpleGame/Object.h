#pragma once

class Object
{
	float x, y, z;
	float size;
	float r, g, b, a;
public:
	Object();
	Object(float x, float y, float z, float size, float r, float g, float b, float a);
	~Object();

	float getX() { return x; };
	float getY() { return y; };
	float getZ() { return z; };
	float getSize() { return size; };
	float getR() { return r; };
	float getG() { return g; };
	float getB() { return b; };
};

