#pragma once

#include <iostream>
#include "Object.h"
#include "Renderer.h"
#define MAX_OBJECTS_COUNT 10

class SceneMgr
{
	Renderer *m_Renderer;
	Object *m_object[MAX_OBJECTS_COUNT];
public:
	SceneMgr();
	~SceneMgr();

	void Add(float x, float y, float z, int size);
	void Update(float elapsedTime);
	void DrawSolidRect();
	bool CollisionTest(float left, float bottom, float right, float top, float left1, float bottom1, float right1, float top1);
};

