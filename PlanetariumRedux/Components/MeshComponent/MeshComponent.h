#pragma once

#include "../RenderingComponent.h"

class Object;

class MeshComponent : public RenderingComponent 
{
public:
	MeshComponent(EngineObject* engineObject, const char* file);
	std::vector<Object> getMesh();
	unsigned int getType();
	void loadMesh() override;
	void setUpMesh() override;
	void renderMesh(unsigned int shader) override;

private:
	unsigned int shaderProgram;
	std::vector<Object> objs;
	const char* filename;
};