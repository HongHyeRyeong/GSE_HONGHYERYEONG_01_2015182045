#pragma once

#include "Object.h"
#define MAX_OBJECTS_COUNT 100

class SceneMgr
{
	Object *m_object[MAX_OBJECTS_COUNT];
	int objectNum;
public:
	SceneMgr();
	~SceneMgr();

	void Add(float x, float y, float z, int size);
	void Update();

	int getNum() { return objectNum; }
	float getX(int i) { return m_object[i]->getX(); }
	float getY(int i) { return m_object[i]->getY(); };
	float getZ(int i) { return m_object[i]->getX(); };
	int getSize(int i) { return m_object[i]->getSize(); };
};

