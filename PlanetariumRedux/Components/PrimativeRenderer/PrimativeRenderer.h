#pragma once

#include "../RenderingComponent.h"
#include "../../Engine/Renderer/ShaderManager/ShaderManager.h"

class Object;
class SunsetShader;
class Plane : public RenderingComponent
{
public:


	Plane(EngineObject* engineObject);
	void loadMesh() override;
	void setUpMesh() override;
	void renderMesh() override;

	unsigned int VAO;
	unsigned int VBO;

	float width = 1.0f; 
	float length = 1.0f; 

	SunsetShader sunsetShader;
};