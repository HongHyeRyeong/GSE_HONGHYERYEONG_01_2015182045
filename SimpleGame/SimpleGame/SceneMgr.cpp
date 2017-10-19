#include "stdafx.h"
#include "SceneMgr.h"

SceneMgr::SceneMgr()
{
	objectNum = 0;

	for (int i = 0; i < 50; ++i)
		m_object[objectNum++] = new Object(i * 20, i * 10, 0, i * 5);
}

SceneMgr::~SceneMgr()
{
	for (int i = 0; i < objectNum; ++i)
		delete m_object[i];
}

void SceneMgr::Add(float x, float y, float z, int size)
{
	if (objectNum < MAX_OBJECTS_COUNT - 1)
		m_object[objectNum++] = new Object(x, y, z, size);
}

void SceneMgr::Update()
{
	for (int i = 0; i < objectNum; ++i)
		m_object[i]->Update();
}