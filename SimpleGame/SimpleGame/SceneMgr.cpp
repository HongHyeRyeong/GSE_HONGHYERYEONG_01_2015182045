#include "stdafx.h"
#include "SceneMgr.h"

SceneMgr::SceneMgr()
{
	m_Renderer = new Renderer(500, 500);
	if (!m_Renderer->IsInitialized())
		std::cout << "Renderer could not be initialized.. \n";
	
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		m_object[i] = NULL;
}

SceneMgr::~SceneMgr()
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		delete m_object[i];

	delete m_Renderer;
}

void SceneMgr::Add(float x, float y, float z, int size)
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		if (m_object[i] == NULL) {
			m_object[i] = new Object(x, y, z, size);
			break;
		}
}

void SceneMgr::Update(float elapsedTime)
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		if (m_object[i] != NULL)
			m_object[i]->Update(elapsedTime);

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) {
		bool isCol = false;

		if (m_object[i] != NULL) {
			for (int j = 0; j < MAX_OBJECTS_COUNT; ++j) {
				if (i == j)
					continue;

				if (m_object[j] != NULL) {
					float size = m_object[i]->getSize() / 2;
					float size1 = m_object[j]->getSize() / 2;

					if (CollisionTest(m_object[i]->getX() - size, m_object[i]->getY() - size, m_object[i]->getX() + size, m_object[i]->getY() + size,
						m_object[j]->getX() - size1, m_object[j]->getY() - size1, m_object[j]->getX() + size1, m_object[j]->getY() + size1))
						isCol = true;
				}
			}
			if (isCol)
			{
				m_object[i]->setRGB(0, 1);
				m_object[i]->setRGB(1, 0);
				m_object[i]->setRGB(2, 0);
			}
			else {
				m_object[i]->setRGB(0, 1);
				m_object[i]->setRGB(1, 1);
				m_object[i]->setRGB(2, 1);
			}
		}
	}
}

void SceneMgr::DrawSolidRect()
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		if (m_object[i] != NULL)
			m_Renderer->DrawSolidRect(m_object[i]->getX(), m_object[i]->getY(), m_object[i]->getZ(),
				m_object[i]->getSize(), m_object[i]->getRGB(0), m_object[i]->getRGB(1), m_object[i]->getRGB(2), 1);
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