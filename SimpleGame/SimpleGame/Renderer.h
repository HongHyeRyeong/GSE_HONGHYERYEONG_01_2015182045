#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a, float level);
	void DrawSolidRectXY(float x, float y, float z, float width, float height, float r, float g, float b, float a, float level);
	void DrawSolidRectGauge(float x, float y, float z, float width, float height, float r, float g, float b, float a, float gauge, float level);
	void DrawBorderXY(float x, float y, float z, float width, float height, float r, float g, float b, float a, float level);
	void DrawTexturedRect(float x, float y, float z, float size, float r, float g, float b, float a, GLuint texID, float level);
	void DrawTexturedRectSeq(float x, float y, float z, float size, float r, float g, float b, float a, GLuint texID, int currSeq, int totalSeq, float level);
	unsigned char * loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight, bool flipY);

	GLuint CreatePngTexture(char * filePath);
	void DeleteTexture(GLuint textureID);

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_VBORectTex = 0;
	GLuint m_VBORectBorder = 0;
	GLuint m_SolidRectShader = 0; 
	GLuint m_SolidRectGaugeShader = 0;
	GLuint m_SolidRectXYShader = 0;
	GLuint m_SolidRectWithTextureShader = 0;
	GLuint m_SolidRectWithTextureSeqShader = 0;

	GLuint m_TextureCharacter = 0;
	GLuint m_TextureBuilding = 0;
	GLuint m_TextureBullet = 0;
	GLuint m_TextureArrow = 0;
};

