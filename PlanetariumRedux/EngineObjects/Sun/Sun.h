#pragma once

#include "../../SunsetCore.h"

class tSun : public EngineObject 
{
public:
	tSun(Scene* scene);
	float star_class;
	Vector3f colour = 1.f; 
};