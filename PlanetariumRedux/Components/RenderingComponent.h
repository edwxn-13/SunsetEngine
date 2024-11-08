#pragma once

#include "Component.h"

class RenderingComponent : public Component 
{
public:
	virtual void loadMesh();
	virtual void setUpMesh();
	virtual void renderMesh();
	RenderingComponent(EngineObject * engineObject);
};