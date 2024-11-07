#pragma once

#include "../RenderingComponent.h"

class Object;
class SunsetShader*;
class PrimativeRenderer : public RenderingComponent
{
public:
	PrimativeRenderer(EngineObject* engineObject);
	void loadMesh() override;
	void setUpMesh() override;
	void renderMesh() override;

private:
	SunsetShader * shader;
};