#pragma once

#include "../RenderingComponent.h"

class Object;

class MeshComponent : public RenderingComponent 
{
public:
	MeshComponent(EngineObject* engineObject, const char* file, unsigned int shader);
	std::vector<Object> getMesh();
	unsigned int getType();
	void loadMesh() override;
	void setUpMesh() override;
	void renderMesh() override;

private:
	unsigned int shaderProgram;
	std::vector<Object> objs;
	const char* filename;
};