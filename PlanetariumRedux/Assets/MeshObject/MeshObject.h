#pragma once
#include "../../EngineObjects/EngineObject.h"

class MeshComponent;
class Scene;

class MeshObject : public EngineObject 
{
public:
	MeshObject(const char* filename, Scene * s);

};