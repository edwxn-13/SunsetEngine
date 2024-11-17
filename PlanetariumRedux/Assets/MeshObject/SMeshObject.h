#pragma once
#include "../../EngineObjects/EngineObject.h"


class Scene;

class SMeshObject : public EngineObject
{
public:
	SMeshObject(const char* filename, Scene* s);

};