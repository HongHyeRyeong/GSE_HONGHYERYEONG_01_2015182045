#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_SolidRectXYShader = CompileShaders("./Shaders/SolidRectXY.vs", "./Shaders/SolidRectXY.fs");
	m_SolidRectGaugeShader = CompileShaders("./Shaders/SolidRectGauge.vs", "./Shaders/SolidRectGauge.fs");
	m_SolidRectWithTextureShader = CompileShaders("./Shaders/SolidRectWithTexture.vs", "./Shaders/SolidRectWithTexture.fs");
	m_SolidRectWithTextureSeqShader = CompileShaders("./Shaders/SolidRectWithTextureSeq.vs", "./Shaders/SolidRectWithTextureSeq.fs");
	
	//Create VBOs
	CreateVertexBufferObjects();

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

GLuint Renderer::CreatePngTexture(char * filePath)
{

	GLuint temp;
	glGenTextures(1, &temp);

	//Load Pngs
	// Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

void Renderer::DeleteTexture(GLuint textureID)
{
	glDeleteTextures(1, &textureID);
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, 
		-1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f,
		 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, 
		 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f,
		 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float rectTex[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, 0.f, 0.f,
		-1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 0.f, 1.f,
		 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f, 1.f,        //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, 0.f, 0.f,
		 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f, 1.f,
		 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, 1.f, 0.f,        //Triangle2
	};

	glGenBuffers(1, &m_VBORectTex);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectTex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectTex), rectTex, GL_STATIC_DRAW);

	float rectBorder[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,
		-1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f,
		1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 
		1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,
	};

	glGenBuffers(1, &m_VBORectBorder);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectBorder);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectBorder), rectBorder, GL_STATIC_DRAW);
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a, float level)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	GLuint shader = m_SolidRectShader;

	//Program select
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glUniform4f(glGetUniformLocation(shader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(shader, "u_Color"), r, g, b, a);
	glUniform1f(glGetUniformLocation(shader, "u_Depth"), level);

	
	
	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawSolidRectXY(float x, float y, float z, float width, float height, float r, float g, float b, float a, float level)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	GLuint shader = m_SolidRectXYShader;

	//Program select
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glUniform4f(glGetUniformLocation(shader, "u_Trans"), newX, newY, width, height);
	glUniform4f(glGetUniformLocation(shader, "u_Color"), r, g, b, a);
	glUniform1f(glGetUniformLocation(shader, "u_Depth"), level);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawSolidRectGauge(float x, float y, float z, float width, float height, float r, float g, float b, float a, float gauge, float level)
{
	DrawBorderXY(x, y, z, width, height, 1, 1, 1, 0.5, level);

	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	GLuint shader = m_SolidRectGaugeShader;

	//Program select
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint u_Trans = glGetUniformLocation(shader, "u_Trans");
	GLuint u_Color = glGetUniformLocation(shader, "u_Color");
	GLuint u_Gauge = glGetUniformLocation(shader, "u_Gauge");
	glUniform1f(glGetUniformLocation(shader, "u_Depth"), level);

	glUniform4f(u_Trans, newX, newY, width, height);
	glUniform4f(u_Color, r, g, b, a);
	glUniform1f(u_Gauge, gauge);

	GLuint attribPosition = glGetAttribLocation(shader, "a_Position");
	GLuint attribTexPosition = glGetAttribLocation(shader, "a_TexPosition");
	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTexPosition);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectTex);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTexPosition, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTexPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawBorderXY(float x, float y, float z, float width, float height, float r, float g, float b, float a, float level)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	GLuint shader = m_SolidRectXYShader;

	//Program select
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glUniform4f(glGetUniformLocation(shader, "u_Trans"), newX, newY, width, height);
	glUniform4f(glGetUniformLocation(shader, "u_Color"), r, g, b, a);
	glUniform1f(glGetUniformLocation(shader, "u_Depth"), level);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectBorder);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawTexturedRect(float x, float y, float z, float size, float r, float g, float b, float a, GLuint texID, float level)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	GLuint shader = m_SolidRectWithTextureShader;

	//Program select
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint u_Trans = glGetUniformLocation(shader, "u_Trans");
	GLuint u_Color = glGetUniformLocation(shader, "u_Color");
	glUniform1f(glGetUniformLocation(shader, "u_Depth"), level);

	glUniform4f(u_Trans, newX, newY, 0, size);
	glUniform4f(u_Color, r, g, b, a);

	GLuint attribPosition = glGetAttribLocation(shader, "a_Position");
	GLuint attribTexPosition = glGetAttribLocation(shader, "a_TexPosition");
	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTexPosition);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectTex);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTexPosition, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	GLuint u_Texture = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(u_Texture, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTexPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawTexturedRectSeq(float x, float y, float z, float size, float r, float g, float b, float a, GLuint texID, int currSeq, int totalSeq, float level)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	GLuint shader = m_SolidRectWithTextureSeqShader;

	//Program select
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint u_Trans = glGetUniformLocation(shader, "u_Trans");
	GLuint u_Color = glGetUniformLocation(shader, "u_Color");
	GLuint u_TotalSeq = glGetUniformLocation(shader, "u_TotalSeq");
	GLuint u_CurrSeq = glGetUniformLocation(shader, "u_CurrSeq");
	glUniform1f(glGetUniformLocation(shader, "u_Depth"), level);

	glUniform4f(u_Trans, newX, newY, 0, size);
	glUniform4f(u_Color, r, g, b, a);
	glUniform1f(u_TotalSeq, (float)totalSeq);
	glUniform1f(u_CurrSeq, (float)currSeq);

	GLuint attribPosition = glGetAttribLocation(shader, "a_Position");
	GLuint attribTexPosition = glGetAttribLocation(shader, "a_TexPosition");
	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTexPosition);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectTex);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTexPosition, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	GLuint u_Texture = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(u_Texture, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTexPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight, bool flipY) 
{
	printf("Reading image %s\n", imagepath);
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file;
	fopen_s(&file, imagepath, "rb");
	if (!file) { printf("Image could not be opened\n"); return NULL; }

	// Read the header, i.e. the 54 first bytes

	// If less than 54 byes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return NULL;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return NULL;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    return NULL; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    return NULL; }

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = outWidth*outHeight * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

										 // Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file wan be closed
	fclose(file);

	if (flipY) {
		// swap y-axis
		unsigned char * tmpBuffer = new unsigned char[outWidth * 3];
		int size = outWidth * 3;
		for (int i = 0; i<outHeight / 2; i++) {
			// copy row i to tmp
			memcpy_s(tmpBuffer, size, data + outWidth * 3 * i, size);
			// copy row h-i-1 to i
			memcpy_s(data + outWidth * 3 * i, size, data + outWidth * 3 * (outHeight - i - 1), size);
			// copy tmp to row h-i-1
			memcpy_s(data + outWidth * 3 * (outHeight - i - 1), size, tmpBuffer, size);
		}
		delete[] tmpBuffer;
	}

	return data;
}
