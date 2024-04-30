#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include "assert.h"

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
	m_ParticleShader = CompileShaders("./Shaders/Particle.vs", "./Shaders/Particle.fs");	// 파티클 쉐이더 사용
	m_ParticleCloudShader = CompileShaders("./Shaders/ParticleCloud.vs", "./Shaders/ParticleCloud.fs");
	m_FSSandboxShader = CompileShaders("./Shaders/FSSandbox.vs", "./Shaders/FSSandbox.fs");
	m_GridMeshShader = CompileShaders("./Shaders/GridMesh.vs", "./Shaders/GridMesh.fs");
	m_TextureSandboxShader = CompileShaders("./Shaders/TextureSandbox.vs", "./Shaders/TextureSandbox.fs");
	
	//Create VBOs
	CreateVertexBufferObjects();

	//Create Paricle Cloud
	CreateParticleCloud(5000);

	//Create Grid Mesh
	CreateGridMesh(32, 32);

	//
	m_RGBTexture = CreatePngTexture("./rgb.png", GL_NEAREST);
	m_NumberTexture[0] = CreatePngTexture("./Textures/0.png", GL_NEAREST);
	m_NumberTexture[1] = CreatePngTexture("./Textures/1.png", GL_NEAREST);
	m_NumberTexture[2] = CreatePngTexture("./Textures/2.png", GL_NEAREST);
	m_NumberTexture[3] = CreatePngTexture("./Textures/3.png", GL_NEAREST);
	m_NumberTexture[4] = CreatePngTexture("./Textures/4.png", GL_NEAREST);
	m_NumberTexture[5] = CreatePngTexture("./Textures/5.png", GL_NEAREST);
	m_NumberTexture[6] = CreatePngTexture("./Textures/6.png", GL_NEAREST);
	m_NumberTexture[7] = CreatePngTexture("./Textures/7.png", GL_NEAREST);
	m_NumberTexture[8] = CreatePngTexture("./Textures/8.png", GL_NEAREST);
	m_NumberTexture[9] = CreatePngTexture("./Textures/9.png", GL_NEAREST);
	m_NumbersTexture = CreatePngTexture("./Textures/numbers.png", GL_NEAREST);

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	//float vertices[] = { 
	//	-0.1f, 0.1f, 0.0f,
	//	-0.1f, -0.1f, 0.0f,
	//	0.1f, 0.1f, 0.0f };

	//glGenBuffers(1, &m_TestVBO);					//	m_TestVBO에 특정한 아이디가 만들어져 들어가게됨
	//glBindBuffer(GL_ARRAY_BUFFER, m_TestVBO);		// 작업대에 VBO를 올려놓음
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	float size = 0.05;
	float particleVerts[] = {
		-size, -size, 0.0f,
		size, size, 0.0f,
		-size, size, 0.0f,
		-size, -size, 0.0f,
		size, -size, 0.0f,
		size, size, 0.0f
	};
	//float* a = new float[100];							// X -> sizeof(a)를 하면 포인터의 크기를 넘겨줌

	glGenBuffers(1, &m_ParticleVBO);					//	m_TestVBO에 특정한 아이디가 만들어져 들어가게됨
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);		// 작업대에 VBO를 올려놓음
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleVerts), particleVerts, GL_STATIC_DRAW);
	
	size = 0.5f;
	float FSSandboxVerts[] = {
		-size, -size, 0.0f,
		size, size, 0.0f,
		-size, size, 0.0f,
		-size, -size, 0.0f,
		size, -size, 0.0f,
		size, size, 0.0f
	};
	//float* a = new float[100];							// X -> sizeof(a)를 하면 포인터의 크기를 넘겨줌

	glGenBuffers(1, &m_FSSandboxVBO);					//	m_TestVBO에 특정한 아이디가 만들어져 들어가게됨
	glBindBuffer(GL_ARRAY_BUFFER, m_FSSandboxVBO);		// 작업대에 VBO를 올려놓음
	glBufferData(GL_ARRAY_BUFFER, sizeof(FSSandboxVerts), FSSandboxVerts, GL_STATIC_DRAW);

	size = 0.5f;
	float TextureSandboxVerts[] = {
		-size, -size, 0, 0, 1,
		size, size, 0, 1, 0,
		-size, size, 0, 0, 0,
		-size, -size, 0, 0, 1,
		size, -size, 0, 1, 1,
		size, size, 0, 1, 0
	};
	//float* a = new float[100];							// X -> sizeof(a)를 하면 포인터의 크기를 넘겨줌

	glGenBuffers(1, &m_TextureSandboxVBO);					//	m_TestVBO에 특정한 아이디가 만들어져 들어가게됨
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);		// 작업대에 VBO를 올려놓음
	glBufferData(GL_ARRAY_BUFFER, sizeof(TextureSandboxVerts), TextureSandboxVerts, GL_STATIC_DRAW);



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
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done." << std::endl;

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::CreateParticleCloud(int numParticles)
{
	float centerX, centerY;
	centerX = 0.f;
	centerY = 0.f;
	float size = 0.005;
	int particleCount = numParticles;
	int vertexCount = particleCount * 6;
	int floatCount = vertexCount * (3 + 1 + 3 + 1 + 1 + 1 + 1 + 4);								
	// x, y, z, startTime, Vx, Vy, Vz, lifeTime, amp, period, value, r, g, b, a
	float VX, VY, YZ;
	float lifeTime;
	float amp, period;
	float value;
	float r, g, b, a;

	float* vertices = NULL;
	vertices = new float[floatCount];

	int index = 0;
	for (int i = 0; i < particleCount; ++i) {
		float velocityScale = 0.2f;
		//centerX = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;		// -1 ~ 1
		//centerY = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;		// -1 ~ 1
		centerX = 0;
		centerY = 0;
		VX = 0.f; //(((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * velocityScale;
		VY = -0.01f; //(((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * velocityScale;
		YZ = (((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * velocityScale;
		//VX = 1.f;
		//VY = 0.5f;
		//YZ = 0.f;
		float StartTime = 8.f * ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		//lifeTime = 8.f * ((float)rand() / (float)RAND_MAX) + 1.f;
		lifeTime = 1.f * ((float)rand() / (float)RAND_MAX) + 1.f;
		//lifeTime = 2.f;
		amp = (((float)rand() / (float)RAND_MAX) - 0.5f) * 2.f;
		period = ((float)rand() / (float)RAND_MAX);
		value = ((float)rand() / (float)RAND_MAX);
		r = ((float)rand() / (float)RAND_MAX);
		g = ((float)rand() / (float)RAND_MAX);
		b = ((float)rand() / (float)RAND_MAX);
		a = ((float)rand() / (float)RAND_MAX);

		vertices[index] = centerX - size; ++index;
		vertices[index] = centerY - size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = StartTime; ++index;
		vertices[index] = VX; ++index;
		vertices[index] = VY; ++index;
		vertices[index] = YZ; ++index;
		vertices[index] = lifeTime; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = period; ++index;
		vertices[index] = value; ++index;
		vertices[index] = r; ++index;
		vertices[index] = g; ++index;
		vertices[index] = b; ++index;
		vertices[index] = a; ++index;

		vertices[index] = centerX + size; ++index;
		vertices[index] = centerY + size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = StartTime; ++index;
		vertices[index] = VX; ++index;
		vertices[index] = VY; ++index;
		vertices[index] = YZ; ++index;
		vertices[index] = lifeTime; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = period; ++index;
		vertices[index] = value; ++index;
		vertices[index] = r; ++index;
		vertices[index] = g; ++index;
		vertices[index] = b; ++index;
		vertices[index] = a; ++index;

		vertices[index] = centerX - size; ++index;
		vertices[index] = centerY + size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = StartTime; ++index;
		vertices[index] = VX; ++index;
		vertices[index] = VY; ++index;
		vertices[index] = YZ; ++index;	
		vertices[index] = lifeTime; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = period; ++index;
		vertices[index] = value; ++index;
		vertices[index] = r; ++index;
		vertices[index] = g; ++index;
		vertices[index] = b; ++index;
		vertices[index] = a; ++index;
		//triangle1

		vertices[index] = centerX - size; ++index;
		vertices[index] = centerY - size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = StartTime; ++index;
		vertices[index] = VX; ++index;
		vertices[index] = VY; ++index;
		vertices[index] = YZ; ++index;
		vertices[index] = lifeTime; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = period; ++index;
		vertices[index] = value; ++index;
		vertices[index] = r; ++index;
		vertices[index] = g; ++index;
		vertices[index] = b; ++index;
		vertices[index] = a; ++index;

		vertices[index] = centerX + size; ++index;
		vertices[index] = centerY - size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = StartTime; ++index;
		vertices[index] = VX; ++index;
		vertices[index] = VY; ++index;
		vertices[index] = YZ; ++index;
		vertices[index] = lifeTime; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = period; ++index;
		vertices[index] = value; ++index;
		vertices[index] = r; ++index;
		vertices[index] = g; ++index;
		vertices[index] = b; ++index;
		vertices[index] = a; ++index;

		vertices[index] = centerX + size; ++index;
		vertices[index] = centerY + size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = StartTime; ++index;
		vertices[index] = VX; ++index;
		vertices[index] = VY; ++index;
		vertices[index] = YZ; ++index;	
		vertices[index] = lifeTime; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = period; ++index;
		vertices[index] = value; ++index;
		vertices[index] = r; ++index;
		vertices[index] = g; ++index;
		vertices[index] = b; ++index;
		vertices[index] = a; ++index;
		//triangle2
	}

	glGenBuffers(1, &m_ParticleCloudVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices, GL_STATIC_DRAW);
	m_ParticleCloudVertexCount = vertexCount;

	delete[] vertices;
}

void Renderer::CreateGridMesh(int x, int y)
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = x;
	int pointCountY = y;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];
	m_GridMeshVertexCount = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_GridMeshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);
}

GLuint Renderer::CreatePngTexture(char* filePath, GLuint samplingMethod)
{
	//Load Png

	std::vector<unsigned char> image;
	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, filePath);

	if (error != 0)

	{

		std::cout << "PNG image loading failed:" << filePath << std::endl;

		assert(0);

	}

	GLuint temp;
	glGenTextures(1, &temp);
	glBindTexture(GL_TEXTURE_2D, temp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, &image[0]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);

	return temp;
}

void Renderer::DrawTest()
{
	//Program select
	glUseProgram(m_SolidRectShader);															// 

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);													// attribPosition을 쓸것이다

	glBindBuffer(GL_ARRAY_BUFFER, m_TestVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);			// 3-> float포인트의 개수, 0 -> GL_ARRAY_BUFFER의 시작점부터 읽어라, sizeof(float) * 3 -> 3으로 나눠짐

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::DrawParticle()
{
	//Program select
	GLuint shader = m_ParticleShader;
	glUseProgram(shader);															// 

	int ulTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(ulTime, m_ParticleTime);
	m_ParticleTime += 0.016;

	int ulPeriod = glGetUniformLocation(shader, "u_Period");
	glUniform1f(ulPeriod, 8.0);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);													// attribPosition을 쓸것이다

	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);			// 3-> float포인트의 개수, 0 -> GL_ARRAY_BUFFER의 시작점부터 읽어라, sizeof(float) * 3 -> 3으로 나눠짐

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::DrawParticleCloud()
{
	glEnable(GL_BLEND);	//alphaBlending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Program select
	GLuint shader = m_ParticleCloudShader;
	glUseProgram(shader);			
	GLuint stride = sizeof(float) * 15;

	int ulTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(ulTime, m_ParticleTime);
	m_ParticleTime += 0.016;

	int ulPeriod = glGetUniformLocation(shader, "u_Period");
	glUniform1f(ulPeriod, 2.0);

	int ulAcc = glGetUniformLocation(shader, "u_Acc");
	glUniform2f(ulAcc, cos(m_ParticleTime), sin(m_ParticleTime));

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);													// attribPosition을 쓸것이다
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);			// 3-> float포인트의 개수, 0 -> GL_ARRAY_BUFFER의 시작점부터 읽어라, sizeof(float) * 3 -> 3으로 나눠짐

	int attribStartTime = glGetAttribLocation(shader, "a_StartTime");
	glEnableVertexAttribArray(attribStartTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribStartTime, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 3));		// 마지막 para -> 어디서부터 정보가 시작되는지.  x, y, z, startTime. startTime은 3번 건너 뛴다음이기 때문에

	int attribVelocity = glGetAttribLocation(shader, "a_Velocity");
	glEnableVertexAttribArray(attribVelocity);													
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribVelocity, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 4));

	int attribLifeTime = glGetAttribLocation(shader, "a_LifeTime");
	glEnableVertexAttribArray(attribLifeTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribLifeTime, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 7));

	int attribAmp = glGetAttribLocation(shader, "a_Amp");
	glEnableVertexAttribArray(attribAmp);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribAmp, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 8));

	int attribPeriod = glGetAttribLocation(shader, "a_Period");
	glEnableVertexAttribArray(attribPeriod);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribPeriod, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 9));

	int attribValue = glGetAttribLocation(shader, "a_Value");
	glEnableVertexAttribArray(attribValue);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribValue, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 10));

	int attribColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 11));
	

	glDrawArrays(GL_TRIANGLES, 0, m_ParticleCloudVertexCount);

	glDisableVertexAttribArray(attribPosition);

	glDisable(GL_BLEND);
}

void Renderer::DrawFSSandbox()
{
	//glEnable(GL_BLEND);	//alphaBlending
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Program select
	GLuint shader = m_FSSandboxShader;
	glUseProgram(shader);
	GLuint stride = sizeof(float) * 3;

	int ulTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(ulTime, m_FSSandboxTime);
	//m_ParticleTime += 0.016;
	m_FSSandboxTime += 0.016;

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);													// attribPosition을 쓸것이다
	glBindBuffer(GL_ARRAY_BUFFER, m_FSSandboxVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);			// 3-> float포인트의 개수, 0 -> GL_ARRAY_BUFFER의 시작점부터 읽어라, sizeof(float) * 3 -> 3으로 나눠짐


	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glDisable(GL_BLEND);
}

void Renderer::DrawGridMesh()
{
	//Program select
	GLuint shader = m_GridMeshShader;
	glUseProgram(shader);															// 

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);													// attribPosition을 쓸것이다

	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);			// 3-> float포인트의 개수, 0 -> GL_ARRAY_BUFFER의 시작점부터 읽어라, sizeof(float) * 3 -> 3으로 나눠짐

	GLuint ul_Time = glGetUniformLocation(shader, "u_Time");
	glUniform1f(ul_Time, m_GridMeshTime);
	m_GridMeshTime += 0.016;

	glDrawArrays(GL_LINE_STRIP, 0, m_GridMeshVertexCount);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::DrawTextureSandbox()
{
	//Program select
	GLuint shader = m_TextureSandboxShader;
	glUseProgram(shader);			
	GLuint stride = sizeof(float) * 5;

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);													// attribPosition을 쓸것이다
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);			// 3-> float포인트의 개수, 0 -> GL_ARRAY_BUFFER의 시작점부터 읽어라, sizeof(float) * 3 -> 3으로 나눠짐

	int attribTexture = glGetAttribLocation(shader, "a_Texture");
	glEnableVertexAttribArray(attribTexture);													// attribPosition을 쓸것이다
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glVertexAttribPointer(attribTexture, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 3));			// 3-> float포인트의 개수, 0 -> GL_ARRAY_BUFFER의 시작점부터 읽어라, sizeof(float) * 3 -> 3으로 나눠짐


	GLuint ul_Time = glGetUniformLocation(shader, "u_Time");
	glUniform1f(ul_Time, m_TextureSandboxTime);
	m_TextureSandboxTime += 0.016;

	int ul_Texture = glGetUniformLocation(shader, "u_Texture");
	//glUniform1i(ul_Texture, 0);
	glUniform1i(ul_Texture, 0);
	glActiveTexture(GL_TEXTURE0);		// 0번 슬롯
	glBindTexture(GL_TEXTURE_2D, m_RGBTexture);

	int ul_Texture1 = glGetUniformLocation(shader, "u_NumberTexture");
	//glUniform1i(ul_Texture, 0);
	int textures[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	glUniform1iv(ul_Texture1, 10, textures);

	int ul_Texture2 = glGetUniformLocation(shader, "u_NumbersTexture");
	//glUniform1i(ul_Texture, 0);
	glUniform1i(ul_Texture2, 11);

	for (int i = 0; i < 10; ++i)		//Numbertexture bind
	{
		glActiveTexture(GL_TEXTURE1 + i);		
		glBindTexture(GL_TEXTURE_2D, m_NumberTexture[i]);
	}
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, m_NumbersTexture);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}
