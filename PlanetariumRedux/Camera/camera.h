#pragma once
#include "../EngineObjects/EngineObject.h"

class SCamera : public EngineObject
{
public:
	enum Camera_Movement;

	SCamera();
	float radz(float deg);
	void InitCamera();
	float cam_dist = 2.f;
	void LookAt(glm::vec3 target, float distance, float xoffset, float yoffset);
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

	const float MovementSpeed = 5.5f;
	float MouseSensitivity = 1.f;
};


class TCamera
{
public:
	enum Camera_Movement;

	TCamera();
	float radz(float deg);
	void InitCamera();
	float cam_dist = 2.f;
	void MoveAndOrientCamera(TCamera& in, glm::vec3 target, float distance, float xoffset, float yoffset);

	const float MovementSpeed = 5.5f;
	float MouseSensitivity = 1.f;
};
