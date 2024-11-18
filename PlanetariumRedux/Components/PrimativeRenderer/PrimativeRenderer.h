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