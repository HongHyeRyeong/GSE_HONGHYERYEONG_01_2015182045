#pragma once

#include <iostream>
#include <random>
#include "Object.h"
#include "Renderer.h"
#include "Sound.h"
using namespace std;

#define MAX_CHARACTER_COUNT 10
#define MAX_ATTACK_COUNT 100

#define OBJECT_BUILDING 100
#define OBJECT_CHARACTER 101
#define OBJECT_BULLET 102
#define OBJECT_ARROW 103

#define fieldW 500
#define fieldH 800

class SceneMgr
{
	Renderer *m_Renderer;
	Object *m_building[6];
	Object *m_character[MAX_CHARACTER_COUNT];
	Object *m_bullet[MAX_ATTACK_COUNT];
	Object *m_arrow[MAX_ATTACK_COUNT];

	GLuint m_texBack;
	GLuint m_texBuilding1, m_texBuilding2;
	GLuint m_texChara1, m_texChara2;
	GLuint m_texBull;
	int m_texCha1Num[2];
	int m_texCha2Num[2];

	float northTime, southTime;
	float texTime, bullTime;

	Sound *m_sound;
	int soundBG;
public:
	SceneMgr();
	~SceneMgr();

	void Add(float x, float y, int type, int id);
	void Update(float elapsedTime);
	void DrawSolidRect();
	void Collision();
	bool CollisionTest(float left, float bottom, float right, float top, float left1, float bottom1, float right1, float top1);

	float getSouthTime() { return southTime; }
};

