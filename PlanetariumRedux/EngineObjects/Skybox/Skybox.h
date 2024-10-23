#pragma once

/*
class SCamera;

#define WIDTH 1024
#define HEIGHT 768

float skyboxVerts[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//0        7--------6
	 1.0f, -1.0f,  1.0f,//1       /|       /|
	 1.0f, -1.0f, -1.0f,//2      4--------5 |
	-1.0f, -1.0f, -1.0f,//3      | |      | |
	-1.0f,  1.0f,  1.0f,//4      | 3------|-2
	 1.0f,  1.0f,  1.0f,//5      |/       |/
	 1.0f,  1.0f, -1.0f,//6      0--------1
	-1.0f,  1.0f, -1.0f//7
};

unsigned int skyboxIndicies[] =
{
	//right 
	1,2,6,
	6,5,1,

	//left 
	0,4,7,
	7,3,0,

	//top
	4,5,6,
	6,7,4,

	//bottom
	0,3,2,
	2,1,0,

	//back
	0,1,5,
	5,4,0,

	//front
	3,7,6,
	6,2,3
};

struct Skybox
{
	GLuint cubemapTex;
	unsigned int skyboxVBO, skyboxVAO, skyboxEBO;

	Skybox();

	GLuint loadCubemap();

	void setupCubemap();
	void renderCubemap(unsigned int skybox_shader, SCamera* Camera);
};*/