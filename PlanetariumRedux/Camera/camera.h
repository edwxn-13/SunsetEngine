#pragma once
#include "../EngineObjects/EngineObject.h"

class SCamera : public EngineObject
{
	static std::vector<SCamera*> m_cam_list;
	static SCamera* p_camera; //Primary Cam

public:
	enum Camera_Movement;

	SCamera();
	float radz(float deg);

	glm::mat4 getCamViewMatrix();
	glm::mat4 getSkyboxViewMatrix();

	float fov = 50.0f;

	const float MovementSpeed = 5.5f;
	float MouseSensitivity = 1.f;

private:
	glm::mat4 view = glm::mat4(1.0f);
	bool primary_cam = true;
	
};
