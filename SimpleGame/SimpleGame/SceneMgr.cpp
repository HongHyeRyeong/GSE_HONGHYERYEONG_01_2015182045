#include "stdafx.h"
#include "SceneMgr.h"

SceneMgr::SceneMgr()
{
	m_Renderer = new Renderer(500, 500);
	if (!m_Renderer->IsInitialized())
		std::cout << "Renderer could not be initialized.. \n";

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		m_character[i] = NULL;

	for (int i = 0; i < 100; ++i)
		m_bullet[i] = NULL;
}

SceneMgr::~SceneMgr()
{
	delete m_Renderer;

	if (m_building != NULL)
		delete m_building;

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		if (m_character[i] != NULL)
			delete m_character[i];

	for (int i = 0; i < 100; ++i)
		if (m_bullet[i] != NULL)
			delete m_bullet[i];
}

void SceneMgr::Add(float x, float y, int size, int type)
{
	if (type == OBJECT_BUILDING) {
		m_building = new Object(x, y, size, type);
	}
	else if (type == OBJECT_CHARACTER) {
		for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
			if (m_character[i] == NULL) {
				m_character[i] = new Object(x, y, size, type);
				break;
			}
	}
	else if (type == OBJECT_BULLET) {
		for (int i = 0; i < 100; ++i)
			if (m_bullet[i] == NULL) {
				m_bullet[i] = new Object(x, y, size, type);
				break;
			}
	}
}

void SceneMgr::Update(float elapsedTime)
{
	// 오브젝트 업데이트
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		if (m_character[i] != NULL)
			m_character[i]->Update(elapsedTime);

	for (int i = 0; i < 100; ++i)
		if (m_bullet[i] != NULL)
			m_bullet[i]->Update(elapsedTime);

	// 충돌체크
	Collision();

	// 소멸
	if (m_building != NULL&&m_building->getLife() <= 0) {
		delete m_building;
		m_building = NULL;
	}

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) {
		if (m_character[i] != NULL) {
			if (m_character[i]->getLife() <= 0) {
				delete m_character[i];
				m_character[i] = NULL;
			}

			//if (m_character[i]->gerLifeTime() <= 0) {
			//	delete m_character[i];
			//	m_character[i] = NULL;
			//}
		}
	}
}

void SceneMgr::DrawSolidRect()
{
	if (m_building != NULL)
		m_Renderer->DrawSolidRect(m_building->getX(), m_building->getY(), 0,
			m_building->getSize(), m_building->getRGB(0), m_building->getRGB(1), m_building->getRGB(2), 1);

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		if (m_character[i] != NULL)
			m_Renderer->DrawSolidRect(m_character[i]->getX(), m_character[i]->getY(), 0,
				m_character[i]->getSize(), m_character[i]->getRGB(0), m_character[i]->getRGB(1), m_character[i]->getRGB(2), 1);

	for (int i = 0; i < 100; ++i)
		if (m_bullet[i] != NULL)
			m_Renderer->DrawSolidRect(m_bullet[i]->getX(), m_bullet[i]->getY(), 0,
				m_bullet[i]->getSize(), m_bullet[i]->getRGB(0), m_bullet[i]->getRGB(1), m_bullet[i]->getRGB(2), 1);
}

void SceneMgr::Collision()
{
	if (m_building != NULL) {
		int sizeB = m_building->getSize() / 2;

		for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) 
		{
			bool isCol = false;

			if (m_character[i] != NULL) 
			{
				float sizeC = m_character[i]->getSize() / 2;

				if (CollisionTest(m_building->getX() - sizeB, m_building->getY() - sizeB, m_building->getX() + sizeB, m_building->getY() + sizeB,
					m_character[i]->getX() - sizeC, m_character[i]->getY() - sizeC, m_character[i]->getX() + sizeC, m_character[i]->getY() + sizeC))
					isCol = true;
			}

			if (isCol)
			{
				//m_character[i]->setRGB(0, 1);
				//m_character[i]->setRGB(1, 0);
				//m_character[i]->setRGB(2, 0);
				m_building->Collision(m_character[i]->getLife());
				//m_character[i]->Collision(20);
				delete m_character[i];
				m_character[i] = NULL;
			}
			//else {
			//	m_character[i]->setRGB(0, 1);
			//	m_character[i]->setRGB(1, 1);
			//	m_character[i]->setRGB(2, 1);
			//}
		}
	}

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) 
		if (m_character[i] != NULL) 
			for (int j = 0; j < 100; ++j) 
				if (m_bullet[j] != NULL) 
				{
					float size = m_character[i]->getSize() / 2;
					float size1 = m_bullet[j]->getSize() / 2;

					if (CollisionTest(m_character[i]->getX() - size, m_character[i]->getY() - size, m_character[i]->getX() + size, m_character[i]->getY() + size,
						m_bullet[j]->getX() - size1, m_bullet[j]->getY() - size1, m_bullet[j]->getX() + size1, m_bullet[j]->getY() + size1))
					{
						m_character[i]->Collision(20);
						delete m_bullet[j];
						m_bullet[j] = NULL;
					}
				}	
}

bool SceneMgr::CollisionTest(float left, float bottom, float right, float top, float left1, float bottom1, float right1, float top1)
{
	if (left > right1)
		return false;
	if (right < left1)
		return false;
	if (bottom > top1)
		return false;
	if (top < bottom1)
		return false;

	return true;
}

bool SceneMgr::isBuilding()
{
	if (m_building != NULL)
		return true;
	else
		return false;
}