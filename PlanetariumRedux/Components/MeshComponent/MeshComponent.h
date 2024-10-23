#pragma once

#include "../Component.h"

class Object;

class MeshComponent : public Component 
{
public:
	MeshComponent(EngineObject* engineObject, const char* file, unsigned int shader);
	std::vector<Object> getMesh();
	unsigned int getType();
	void loadMesh();
	void setUpMesh();
	void renderMesh();

private:
	unsigned int shaderProgram;
	std::vector<Object> objs;
	const char* filename;
};