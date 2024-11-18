#pragma once

#include "../Component.h"
#include "../../Maths/MathHeader.h"
#include "../../Engine/EngineUtils.h"
class SCamera;

class CameraComponent : public Component 
{
public:
	CameraComponent(EngineObject* engineObject, SCamera * camera);
	void Update(float deltaTime) override;
	void FixedUpdate(float deltaTime) override;
	void Start() override;

private:

	Input input_controller; 
	float m_fov;
	unsigned int layer;
	Vector3f fromt;
	Vector3f former_position;
	SCamera* parent_cam;
	float yaw, pitch = 0;
	float roll = 0.0f;
	float move_speed = 0.8f;
	float look_speed = 1.2f;
};