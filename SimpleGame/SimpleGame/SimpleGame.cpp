#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "SceneMgr.h"
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

SceneMgr *mgr = NULL;

DWORD g_startTime = NULL;
DWORD g_bulletTime = NULL;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	DWORD currTime = timeGetTime();
	DWORD elapsedTime = currTime - g_startTime;
	g_startTime = currTime;

	if (mgr->isBuilding()) {	// 건물이 존재할때만
		currTime = (float)timeGetTime() / 1000;
		if (currTime - g_bulletTime >= 0.5f) {
			mgr->Add(0, 0, 5, OBJECT_BULLET);
			g_bulletTime = currTime;
		}
	}
	
	mgr->Update((float)elapsedTime);
	mgr->DrawSolidRect();

	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mgr->Add(x - 250, 250 - y, 10, OBJECT_CHARACTER);
	}

	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
		std::cout << " GLEW Version is 3.0\n ";
	else
		std::cout << "GLEW 3.0 not supported\n ";

	mgr = new SceneMgr();
	g_startTime = timeGetTime();
	mgr->Add(0, 0, 50, OBJECT_BUILDING);

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	glutMainLoop();

	delete mgr;

	return 0;
}

