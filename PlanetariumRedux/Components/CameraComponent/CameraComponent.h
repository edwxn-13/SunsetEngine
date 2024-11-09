#pragma once

#include "../Component.h"
#include "../../Maths/MathHeader.h"
class CameraComponent : public Component 
{
public:
	CameraComponent(EngineObject* engineObject);
	void Update(float deltaTime) override;
	void FixedUpdate(float deltaTime) override;
	void Start() override;

private:
	float m_fov;
	unsigned int layer;
	Vector3f fromt;
	Vector3f former_position;

	float yaw, pitch = 0;
	float roll = 0.0f;

};