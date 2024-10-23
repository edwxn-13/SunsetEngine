#pragma once
#include "../../EngineObjects/EngineObject.h"

class MeshComponent;

class MeshObject : public EngineObject 
{
public:
	MeshObject(const char* filename, unsigned int shader);
	virtual void Start() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void LateUpdate() override;
};