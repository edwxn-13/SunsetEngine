#pragma once

#include "Component.h"

class RenderingComponent : public Component 
{
	virtual void loadMesh();
	virtual void setUpMesh();
	virtual void renderMesh();

public:
	RenderingComponent(EngineObject * engineObject);
};