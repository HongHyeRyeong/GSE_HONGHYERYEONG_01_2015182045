#pragma once

#include <iostream>
#include "Object.h"
#include "Renderer.h"

#define MAX_OBJECTS_COUNT 10
#define OBJECT_BUILDING 100
#define OBJECT_CHARACTER 101
#define OBJECT_BULLET 102
#define OBJECT_ARROW 103

class SceneMgr
{
	Renderer *m_Renderer;
	Object *m_building;
	Object *m_character[MAX_OBJECTS_COUNT];
	Object *m_bullet[100];
	Object *m_arrow[100];

	GLuint m_texBuilding;
public:
	SceneMgr();
	~SceneMgr();

	void Add(float x, float y, int size, int type);
	void Update(float elapsedTime);
	void DrawSolidRect();
	void Collision();
	bool CollisionTest(float left, float bottom, float right, float top, float left1, float bottom1, float right1, float top1);

	bool isBuilding();
};

