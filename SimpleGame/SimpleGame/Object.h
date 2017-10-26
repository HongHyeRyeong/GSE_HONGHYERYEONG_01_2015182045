#pragma once
#include <cstdlib>

class Object
{
	float x, y, z;
	float size;
	float rgb[3];
	float vX, vY;

public:
	Object() {};
	Object(float x, float y, float z, float size);
	~Object() {};

	void Update(float elapsedTime);

	float getX() { return x; };
	float getY() { return y; };
	float getZ() { return z; };
	float getSize() { return size; };
	float getRGB(int i) { return rgb[i]; }

	void setRGB(int i, int j) { rgb[i] = j; }
};

