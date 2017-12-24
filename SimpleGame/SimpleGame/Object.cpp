#include "stdafx.h"
#include "Object.h"
#include "SceneMgr.h"

Object::Object(float x, float y, float size, int type, int id) :x(x), y(y), size(size), type(type), id(id), attackTime(0)
{
	if (type == OBJECT_BUILDING) {
		life = 500;
		vX = 0;
		vY = 0;

		rgb[0] = 1;
		rgb[1] = 1;
		rgb[2] = 1;
	}
	else if (type == OBJECT_CHARACTER) {
		life = 100;
		vX = 100.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);
		vY = 200.f *((float)std::rand() / (float)RAND_MAX);

		texTime = 0;
		m_texNum[0] = 0;
		m_texNum[1] = 0;

		rgb[0] = 1;
		rgb[1] = 1;
		rgb[2] = 1;

		if (id == 1)
			vY = -vY;
	}
	else if (type == OBJECT_BULLET) {
		life = 15;
		lifeTime = 0.0;
		vX = 200.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);
		vY = 300.f *((float)std::rand() / (float)RAND_MAX);

		if (id == 1) {
			rgb[0] = 1;
			rgb[1] = 0;
			rgb[2] = 0;

			vY = -vY;
		}
		else if (id == 2) {
			rgb[0] = 0;
			rgb[1] = 0;
			rgb[2] = 1;
		}
	}
	else if (type == OBJECT_ARROW) {
		life = 10;
		vX = 100.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);
		vY = 200.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);

		rgb[0] = 1;
		rgb[1] = 1;
		rgb[2] = 1;
	}
	else if (type == OBJECT_EXPLOSION) {
		life = 1;
		vX = 0;
		vY = 0;

		rgb[0] = 1;
		rgb[1] = 1;
		rgb[2] = 1;

		texTime = 0;
		m_texNum[0] = 0;
		m_texNum[1] = 0;
	}
}

void Object::Update(float elapsedTime)
{
	float time = elapsedTime / 1000;
	x += vX * time;
	y += vY * time;
	attackTime += time;
	texTime += time;

	if (type == OBJECT_CHARACTER) {
		if (x >= fieldW / 2 || x <= -fieldW / 2)
			vX = -vX;
		else if (y >= fieldH / 2 || y <= -fieldH / 2)
			vY = -vY;

		if (texTime >= 0.1) {
			UpdateTex();
			texTime = 0;
		}
	}
	else if (type == OBJECT_BULLET || type == OBJECT_ARROW) {
		if (x >= fieldW / 2 || x <= -fieldW / 2 || y >= fieldH / 2 || y <= -fieldH / 2)
			life = 0;

		lifeTime += time;

		if (lifeTime >= 2)
			life = 0;
	}
	else if (type == OBJECT_EXPLOSION) {
		if (texTime >= 0.05) {
			UpdateTex();
			texTime = 0;
		}
	}
}

void Object::UpdateTex()
{
	if (type == OBJECT_CHARACTER) {
		if (id == 1) {
			if (m_texNum[0] == 15)
				m_texNum[0] = 0;
			else
				m_texNum[0]++;
		}
		else {
			if (m_texNum[0] == 5) {
				m_texNum[0] = 0;
				m_texNum[1]++;
			}
			else
				m_texNum[0]++;

			if (m_texNum[0] == 5 && m_texNum[1] == 3) {
				m_texNum[0] = 0;
				m_texNum[1] = 0;
			}
		}
	}
	else if (type == OBJECT_EXPLOSION) {
		if (m_texNum[0] == 3) {
			m_texNum[0] = 0;
			m_texNum[1]++;
		}
		else
			m_texNum[0]++;

		if (m_texNum[0] == 3 && m_texNum[1] == 3)
			life = 0;
	}
}