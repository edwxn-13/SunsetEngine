#pragma once
#include "../EngineObjects/EngineObject.h"

class SCamera : public EngineObject
{
	static std::vector<SCamera*> m_cam_list;
public:
	enum Camera_Movement;

	SCamera();
	float radz(float deg);
	void InitCamera();
	float cam_dist = 2.f;
	void LookAt(glm::vec3 target);
	void Look();

	glm::mat4 getCamViewMatrix();
	glm::mat4 getSkyboxViewMatrix();

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float fov = 50.0f;

	const float MovementSpeed = 5.5f;
	float MouseSensitivity = 1.f;
};
