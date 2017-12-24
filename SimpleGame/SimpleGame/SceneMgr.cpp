#include "stdafx.h"
#include "SceneMgr.h"

SceneMgr::SceneMgr()
{
	m_Renderer = new Renderer(fieldW, fieldH);
	if (!m_Renderer->IsInitialized())
		std::cout << "Renderer could not be initialized.. \n";

	Add(-150, 250, 100, OBJECT_BUILDING, 1);
	Add(0, 300, 130, OBJECT_BUILDING, 1);
	Add(150, 250, 100, OBJECT_BUILDING, 1);
	Add(-150, -250, 100, OBJECT_BUILDING, 2);
	Add(0, -300, 130, OBJECT_BUILDING, 2);
	Add(150, -250, 100, OBJECT_BUILDING, 2);

	for (int i = 0; i < MAX_CHARACTER_COUNT; ++i)
		m_character[i] = NULL;

	for (int i = 0; i < MAX_ATTACK_COUNT; ++i) {
		m_bullet[i] = NULL;
		m_arrow[i] = NULL;
		m_explosion[i] = NULL;
	}

	northTime = 10;
	southTime = 10;
	weatherTime = 0;
	shakeNum = 0;
	shakeV = 1;
	shakeTime = 0;

	m_texBack[0] = m_Renderer->CreatePngTexture("./Resource/배경1.png");
	m_texBack[1] = m_Renderer->CreatePngTexture("./Resource/배경2.png");
	m_texBuilding1[0] = m_Renderer->CreatePngTexture("./Resource/빌딩1.png");
	m_texBuilding1[1] = m_Renderer->CreatePngTexture("./Resource/빌딩3.png");
	m_texBuilding1[2] = m_Renderer->CreatePngTexture("./Resource/빌딩2.png");
	m_texBuilding2[0] = m_Renderer->CreatePngTexture("./Resource/집1.png");
	m_texBuilding2[1] = m_Renderer->CreatePngTexture("./Resource/집2.png");
	m_texBuilding2[2] = m_Renderer->CreatePngTexture("./Resource/집3.png");
	m_texChara1 = m_Renderer->CreatePngTexture("./Resource/기계.png");
	m_texChara2 = m_Renderer->CreatePngTexture("./Resource/동물.png");
	m_texArrow1 = m_Renderer->CreatePngTexture("./Resource/전기.png");
	m_texArrow2 = m_Renderer->CreatePngTexture("./Resource/꽃.png");
	m_texBull = m_Renderer->CreatePngTexture("./Resource/particle1.png");
	m_texExplosion = m_Renderer->CreatePngTexture("./Resource/폭발.png");

	m_soundBG = new Sound();
	soundBG = m_soundBG->CreateSound("./Dependencies/SoundSamples/Hackbeat.mp3");
	m_soundBG->PlaySound(soundBG, true, 0.4f);
	m_soundEffect = new Sound();
	soundEffect = m_soundEffect->CreateSound("./Dependencies/SoundSamples/explosion.wav");
	m_soundEffect2 = new Sound();
	soundEffect2 = m_soundEffect2->CreateSound("./Dependencies/SoundSamples/Throwing fireball.wav");
	m_soundEffect3 = new Sound();
	soundEffect3 = m_soundEffect3->CreateSound("./Dependencies/SoundSamples/193.wav");
}

SceneMgr::~SceneMgr()
{
	for (int i = 0; i < 6; ++i)
		if (m_building != NULL)
			delete m_building[i];

	for (int i = 0; i < MAX_CHARACTER_COUNT; ++i)
		if (m_character[i] != NULL)
			delete m_character[i];

	for (int i = 0; i < MAX_ATTACK_COUNT; ++i)
		if (m_bullet[i] != NULL)
			delete m_bullet[i];

	for (int i = 0; i < MAX_ATTACK_COUNT; ++i)
		if (m_arrow[i] != NULL)
			delete m_arrow[i];

	for (int i = 0; i < MAX_ATTACK_COUNT; ++i)
		if (m_explosion[i] != NULL)
			delete m_explosion[i];

	delete m_Renderer;
	delete[] m_building;
	delete[] m_character;
	delete[] m_bullet;
	delete[] m_arrow;
	delete[] m_explosion;
}

void SceneMgr::Add(float x, float y, float size, int type, int id)
{
	if (type == OBJECT_BUILDING) {
		for (int i = 0; i < 6; ++i)
			if (m_building[i] == NULL) {
				m_building[i] = new Object(x, y, size, type, id);
				break;
			}
	}
	else if (type == OBJECT_CHARACTER) {
		for (int i = 0; i < MAX_CHARACTER_COUNT; ++i)
			if (m_character[i] == NULL) {
				m_character[i] = new Object(x, y, size, type, id);

				if (id == 2)
					southTime = 0;
				m_soundEffect2->PlaySound(soundEffect2, false, 0.4f);
				break;
			}
	}
	else if (type == OBJECT_BULLET) {
		for (int i = 0; i < MAX_ATTACK_COUNT; ++i)
			if (m_bullet[i] == NULL) {
				m_bullet[i] = new Object(x, y, size, type, id);
				break;
			}
	}
	else if (type == OBJECT_ARROW) {
		for (int i = 0; i < MAX_ATTACK_COUNT; ++i)
			if (m_arrow[i] == NULL) {
				m_arrow[i] = new Object(x, y, size, type, id);
				break;
			}
	}
	else if (type == OBJECT_EXPLOSION) {
		for (int i = 0; i < MAX_ATTACK_COUNT; ++i)
			if (m_explosion[i] == NULL) {
				m_soundEffect3->PlaySound(soundEffect3, false, 0.3f);
				m_explosion[i] = new Object(x, y, size, type, id);
				break;
			}
	}
}

void SceneMgr::Update(float elapsedTime)
{
	// 유닛 배치
	northTime += elapsedTime / 1000;
	southTime += elapsedTime / 1000;
	weatherTime += elapsedTime / 1000;
	shakeTime += elapsedTime / 1000;

	if (northTime >= 3) {
		random_device rd;
		mt19937_64 rng(rd());
		uniform_int_distribution<> uiX(-200, 200);
		uniform_int_distribution<> uiY(150, 300);

		Add(uiX(rng), uiY(rng), 80, OBJECT_CHARACTER, 1);
		northTime = 0;
	}

	// 오브젝트 업데이트
	for (int i = 0; i < 6; ++i)
		if (m_building[i] != NULL) {
			m_building[i]->Update(elapsedTime);

			if (m_building[i]->getAttackTime() >= 2) {
				Add(m_building[i]->getX(), m_building[i]->getY(), 6, OBJECT_BULLET, m_building[i]->getId());
				m_building[i]->setAttackTime(0);
			}
		}

	for (int i = 0; i < MAX_CHARACTER_COUNT; ++i)
		if (m_character[i] != NULL) {
			m_character[i]->Update(elapsedTime);

			if (m_character[i]->getAttackTime() >= 3) {
				Add(m_character[i]->getX(), m_character[i]->getY(), 30, OBJECT_ARROW, m_character[i]->getId());
				m_character[i]->setAttackTime(0);
			}
		}

	for (int i = 0; i < MAX_ATTACK_COUNT; ++i) {
		if (m_bullet[i] != NULL)
			m_bullet[i]->Update(elapsedTime);
		if (m_arrow[i] != NULL)
			m_arrow[i]->Update(elapsedTime);
		if (m_explosion[i] != NULL)
			m_explosion[i]->Update(elapsedTime);
	}

	if (shakeTime >= 0.03) {
		if (shakeV != 0) {
			if (shakeNum == 20)
				shakeV = -1;
			else if (shakeNum == -20) {
				shakeNum = 0;
				shakeV = 0;
			}

			shakeNum += shakeV * 5;
		}
		shakeTime = 0;
	}

	// 충돌체크
	Collision();

	// 소멸
	for (int i = 0; i < 6; ++i)
		if (m_building[i] != NULL && m_building[i]->getLife() <= 0) {
			delete m_building[i];
			m_building[i] = NULL;
			shakeV = 1;
			m_soundEffect->PlaySound(soundEffect, false, 0.4f);
		}

	for (int i = 0; i < MAX_CHARACTER_COUNT; ++i) {
		if (m_character[i] != NULL && m_character[i]->getLife() <= 0) {
			delete m_character[i];
			m_character[i] = NULL;
		}
	}

	for (int i = 0; i < MAX_ATTACK_COUNT; ++i) {
		if (m_bullet[i] != NULL && m_bullet[i]->getLife() <= 0) {
			delete m_bullet[i];
			m_bullet[i] = NULL;
		}
		if (m_arrow[i] != NULL && m_arrow[i]->getLife() <= 0) {
			delete m_arrow[i];
			m_arrow[i] = NULL;
		}
		if (m_explosion[i] != NULL && m_explosion[i]->getLife() <= 0) {
			delete m_explosion[i];
			m_explosion[i] = NULL;
		}
	}
}

void SceneMgr::DrawSolidRect()
{
	m_Renderer->DrawTexturedRect(0, fieldH / 4, 0, fieldW, 1.0, 1.0, 1.0, 1, m_texBack[0], 0.99);
	m_Renderer->DrawTexturedRect(0, -fieldH / 4, 0, fieldW, 1.0, 1.0, 1.0, 1, m_texBack[1], 0.99);

	for (int i = 0; i < MAX_ATTACK_COUNT; ++i)
		if (m_explosion[i] != NULL)
			m_Renderer->DrawTexturedRectSeq(m_explosion[i]->getX(), m_explosion[i]->getY(), 0,
				m_explosion[i]->getSize(), m_explosion[i]->getRGB(0), m_explosion[i]->getRGB(1), m_explosion[i]->getRGB(2), 1, m_texExplosion, m_explosion[i]->getTexNum(0), m_explosion[i]->getTexNum(1), 4, 4, 0.02);

	for (int i = 0; i < 6; ++i)
		if (m_building[i] != NULL) {
			m_Renderer->SetSceneTransform(shakeNum, 0, 1, 1);
			if (m_building[i]->getId() == 1) {
				m_Renderer->DrawTexturedRect(m_building[i]->getX(), m_building[i]->getY(), 0,
					m_building[i]->getSize(), m_building[i]->getRGB(0), m_building[i]->getRGB(1), m_building[i]->getRGB(2), 1, m_texBuilding1[i], 0.1);
				m_Renderer->DrawSolidRectGauge(m_building[i]->getX(), m_building[i]->getY() + 60, 0, 100, 5, 1, 0, 0, 1, m_building[i]->getLife() / 500, 0.1);

			}
			else {
				m_Renderer->DrawTexturedRect(m_building[i]->getX(), m_building[i]->getY(), 0,
					m_building[i]->getSize(), m_building[i]->getRGB(0), m_building[i]->getRGB(1), m_building[i]->getRGB(2), 1, m_texBuilding2[i - 3], 0.1);
				m_Renderer->DrawSolidRectGauge(m_building[i]->getX(), m_building[i]->getY() + 60, 0, 100, 5, 0, 0, 1, 1, m_building[i]->getLife() / 500, 0.1);
			}
			m_Renderer->SetSceneTransform(0, 0, 1, 1);
		}

	for (int i = 0; i < MAX_CHARACTER_COUNT; ++i)
		if (m_character[i] != NULL) {
			if (m_character[i]->getId() == 1) {
				m_Renderer->DrawTexturedRectSeq(m_character[i]->getX(), m_character[i]->getY(), 0,
					m_character[i]->getSize(), m_character[i]->getRGB(0), m_character[i]->getRGB(1), m_character[i]->getRGB(2), 1, m_texChara1, m_character[i]->getTexNum(0), m_character[i]->getTexNum(1), 14, 1, 0.2);
				m_Renderer->DrawSolidRectGauge(m_character[i]->getX(), m_character[i]->getY() + m_character[i]->getSize() / 2, 0, 40, 5, 1, 0, 0, 1, m_character[i]->getLife() / 100, 0.2);
			}
			else {
				m_Renderer->DrawTexturedRectSeq(m_character[i]->getX(), m_character[i]->getY(), 0,
					m_character[i]->getSize(), m_character[i]->getRGB(0), m_character[i]->getRGB(1), m_character[i]->getRGB(2), 1, m_texChara2, m_character[i]->getTexNum(0), m_character[i]->getTexNum(1), 6, 4, 0.2);
				m_Renderer->DrawSolidRectGauge(m_character[i]->getX(), m_character[i]->getY() + m_character[i]->getSize() / 2, 0, 40, 5, 0, 0, 1, 1, m_character[i]->getLife() / 100, 0.2);
			}
		}

	for (int i = 0; i < MAX_ATTACK_COUNT; ++i)
		if (m_bullet[i] != NULL) {
			if (m_bullet[i]->getId() == 1)
				m_Renderer->DrawParticle(m_bullet[i]->getX(), m_bullet[i]->getY(), 0,
					m_bullet[i]->getSize(), m_bullet[i]->getRGB(0), m_bullet[i]->getRGB(1), m_bullet[i]->getRGB(2), 1 - m_bullet[i]->gerLifeTime() / 3, 0, 1, m_texBull, m_bullet[i]->gerLifeTime(), 0.3);
			else
				m_Renderer->DrawParticle(m_bullet[i]->getX(), m_bullet[i]->getY(), 0,
					m_bullet[i]->getSize(), m_bullet[i]->getRGB(0), m_bullet[i]->getRGB(1), m_bullet[i]->getRGB(2), 1 - m_bullet[i]->gerLifeTime() / 2, 0, -1, m_texBull, m_bullet[i]->gerLifeTime(), 0.3);
		}

	for (int i = 0; i < MAX_ATTACK_COUNT; ++i)
		if (m_arrow[i] != NULL) {
			if (m_arrow[i]->getId() == 1)
				m_Renderer->DrawTexturedRect(m_arrow[i]->getX(), m_arrow[i]->getY(), 0, m_arrow[i]->getSize(), m_arrow[i]->getRGB(0), m_arrow[i]->getRGB(1), m_arrow[i]->getRGB(2), 1, m_texArrow1, 0.3);
			else
				m_Renderer->DrawTexturedRect(m_arrow[i]->getX(), m_arrow[i]->getY(), 0, m_arrow[i]->getSize(), m_arrow[i]->getRGB(0), m_arrow[i]->getRGB(1), m_arrow[i]->getRGB(2), 1, m_texArrow2, 0.3);
		}

	m_Renderer->DrawParticleClimate(0, 0, 0, 1, 1, 1, 1, 1, -0.1, -0.1, m_texBull, weatherTime, 0.01);
	m_Renderer->DrawText(-35, 35, GLUT_BITMAP_HELVETICA_18, 0.0, 0.0, 0.0, "City VS Country");
}

void SceneMgr::Collision()
{
	// 빌딩과 캐릭터
	for (int i = 0; i < 6; ++i)
		if (m_building[i] != NULL)
			for (int j = 0; j < MAX_CHARACTER_COUNT; ++j)
				if (m_character[j] != NULL && m_building[i]->getId() != m_character[j]->getId())
				{
					float size = m_building[i]->getSize() / 2;
					float size1 = m_character[j]->getSize() / 2;

					if (CollisionTest(m_building[i]->getX() - size, m_building[i]->getY() - size, m_building[i]->getX() + size, m_building[i]->getY() + size,
						m_character[j]->getX() - size1, m_character[j]->getY() - size1, m_character[j]->getX() + size1, m_character[j]->getY() + size1))
					{
						Add(m_building[i]->getX(), m_building[i]->getY(), 100, OBJECT_EXPLOSION, 99);
						m_building[i]->Collision(100);
						delete m_character[j];
						m_character[j] = NULL;
					}
				}

	// 빌딩과 총알
	for (int i = 0; i < 6; ++i)
		if (m_building[i] != NULL)
			for (int j = 0; j < MAX_ATTACK_COUNT; ++j)
				if (m_bullet[j] != NULL && m_building[i]->getId() != m_bullet[j]->getId())
				{
					float size = m_building[i]->getSize() / 2;
					float size1 = m_bullet[j]->getSize() / 2;

					if (CollisionTest(m_building[i]->getX() - size, m_building[i]->getY() - size, m_building[i]->getX() + size, m_building[i]->getY() + size,
						m_bullet[j]->getX() - size1, m_bullet[j]->getY() - size1, m_bullet[j]->getX() + size1, m_bullet[j]->getY() + size1))
					{
						Add(m_building[i]->getX(), m_building[i]->getY(), 100, OBJECT_EXPLOSION, 99);
						m_building[i]->Collision(m_bullet[j]->getLife());
						delete m_bullet[j];
						m_bullet[j] = NULL;
					}
				}

	// 캐릭터와 총알
	for (int i = 0; i < MAX_CHARACTER_COUNT; ++i)
		if (m_character[i] != NULL)
			for (int j = 0; j < MAX_ATTACK_COUNT; ++j)
				if (m_bullet[j] != NULL && m_character[i]->getId() != m_bullet[j]->getId())
				{
					float size = m_character[i]->getSize() / 2;
					float size1 = m_bullet[j]->getSize() / 2;

					if (CollisionTest(m_character[i]->getX() - size, m_character[i]->getY() - size, m_character[i]->getX() + size, m_character[i]->getY() + size,
						m_bullet[j]->getX() - size1, m_bullet[j]->getY() - size1, m_bullet[j]->getX() + size1, m_bullet[j]->getY() + size1))
					{
						Add(m_character[i]->getX(), m_character[i]->getY(), 50, OBJECT_EXPLOSION, 99);
						m_character[i]->Collision(m_bullet[j]->getLife());
						delete m_bullet[j];
						m_bullet[j] = NULL;
					}
				}

	// 빌딩과 화살
	for (int i = 0; i < 6; ++i)
		if (m_building[i] != NULL)
			for (int j = 0; j < MAX_ATTACK_COUNT; ++j)
				if (m_arrow[j] != NULL && m_building[i]->getId() != m_arrow[j]->getId())
				{
					float size = m_building[i]->getSize() / 2;
					float size1 = m_arrow[j]->getSize() / 2;

					if (CollisionTest(m_building[i]->getX() - size, m_building[i]->getY() - size, m_building[i]->getX() + size, m_building[i]->getY() + size,
						m_arrow[j]->getX() - size1, m_arrow[j]->getY() - size1, m_arrow[j]->getX() + size1, m_arrow[j]->getY() + size1))
					{
						Add(m_building[i]->getX(), m_building[i]->getY(), 100, OBJECT_EXPLOSION, 99);
						m_building[i]->Collision(m_arrow[j]->getLife());
						delete m_arrow[j];
						m_arrow[j] = NULL;
					}
				}

	// 캐릭터와 화살
	for (int i = 0; i < MAX_CHARACTER_COUNT; ++i)
		if (m_character[i] != NULL)
			for (int j = 0; j < 100; ++j)
				if (m_arrow[j] != NULL && m_character[i]->getId() != m_arrow[j]->getId())
				{
					float size = m_character[i]->getSize() / 2;
					float size1 = m_arrow[j]->getSize() / 2;

					if (CollisionTest(m_character[i]->getX() - size, m_character[i]->getY() - size, m_character[i]->getX() + size, m_character[i]->getY() + size,
						m_arrow[j]->getX() - size1, m_arrow[j]->getY() - size1, m_arrow[j]->getX() + size1, m_arrow[j]->getY() + size1))
					{
						Add(m_character[i]->getX(), m_character[i]->getY(), 50, OBJECT_EXPLOSION, 99);
						m_character[i]->Collision(m_arrow[j]->getLife());
						delete m_arrow[j];
						m_arrow[j] = NULL;
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