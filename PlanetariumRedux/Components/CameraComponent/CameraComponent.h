#pragma once

#include "../Component.h"
#include "../../Maths/MathHeader.h"
class CameraComponent : public Component 
{
public:
	CameraComponent(EngineObject* engineObject);
	void Update() override;
	void FixedUpdate() override;
	void Start() override;

private:
	float m_fov;
	unsigned int layer;
	Vector3f fromt;

	float yaw, pitch = 0;
	float roll = 0.0f;

};