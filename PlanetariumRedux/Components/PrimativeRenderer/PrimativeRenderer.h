#pragma once

#include "../RenderingComponent.h"
#include "../../Engine/Renderer/ShaderManager/ShaderManager.h"

class Object;
class SunsetShader;
class PlaneRenderer : public RenderingComponent
{
public:


	PlaneRenderer(EngineObject* engineObject, float &width, float &length);
	void loadMesh() override;
	void setUpMesh() override;
	void renderMesh(unsigned int shader) override;

	unsigned int VAO;
	unsigned int VBO;

	float width = 1.0f; 
	float length = 1.0f; 

	SunsetShader sunsetShader;
};

class CubeRenderer : public RenderingComponent 
{
public:
	CubeRenderer(EngineObject* engineObject, float& width, float& height, float& length);
	void loadMesh() override;
	void setUpMesh() override;
	void renderMesh(unsigned int shader) override;


private:
	unsigned int VAO;	
	unsigned int VBO;
	unsigned int EBO;

	float width = 90.0f;
	float length = 1.0f;
	float height = 90.0f;

	float vertices[24] = 
	{
		-width, -height, -length, // 0
		width, -height, -length, // 1
		width,  height, -length, // 2
		-width,  height, -length, // 3
		-width, -height,  length, // 4
		width, -height,  length, // 5
		width,  height,  length, // 6
		-width,  height,  length // 7
	};

	float texCoords[12] = 
	{ 
		0.0, 0.0, 0.0,
		1.0, 0.0,0.0,
		1.0, 1.0,0.0,
		0.0, 1.0, 0.0
	};

	short indices[36] =
	{ 
		0,2,1,
		0,3,2,

		1,2,6,
		6,5,1,

		4,5,6,
		6,7,4,

		2,3,6,
		6,3,7,

		0,7,3,
		0,4,7,

		0,1,5,
		0,5,4
	};

	float cube_verts[324] =
	{
	-width, -height, -length,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	width, -height, -length,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	width,  height, -length,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	width,  height, -length,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	-width, height, -length,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	-width, -height, -length,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,

	//front face
	-width, -height,  length,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	width, -height,  length,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	width,  height,  length,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	width,  height,  length,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	-width,  height,  length,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	-width, -height,  length,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,

	//left face
	-width, height,  length,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-width, height, -length,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-width, -height, -length,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-width, -height, -length,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-width, -height,  length,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-width,  height,  length,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,

	//right face
	width,  height,  length,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	width,  height, -length,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	width, -height, -length, 	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	width, -height, -length,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	width, -height,  length,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	width,  height,  length,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,

	//bottom face
	-width, -height, -length,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	width, -height, -length,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	width, -height,  length,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	width, -height,  length,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	-width, -height,  length,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	-width, -height, -length,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,

	//top face
	-width,  height, -length,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	width,  height, -length,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	width,  height,  length,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	width,  height,  length,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	-width,  height,  length,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	-width,  height, -length, 	1.0f, 1.f, 1.0f, 	0.f,1.f,0.f
	};

	SunsetShader sunsetShader;
};