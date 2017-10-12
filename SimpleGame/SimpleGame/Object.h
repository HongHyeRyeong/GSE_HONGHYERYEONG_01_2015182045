#pragma once

class Object
{
	float x, y, z;
	int size;
	float vX, vY;

public:
	Object() {};
	Object(float x, float y, float z, int size) :x(x), y(y), z(z), size(size), vX(1.5), vY(-1.5) {};
	~Object() {};

	void Update();

	float getX() { return x; };
	float getY() { return y; };
	float getZ() { return z; };
	int getSize() { return size; };
};

