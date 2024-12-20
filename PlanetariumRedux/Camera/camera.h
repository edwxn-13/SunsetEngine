#pragma once
#include "../EngineObjects/EngineObject.h"

class SCamera : public EngineObject
{
	static std::vector<SCamera*> m_cam_list;
	static SCamera* p_camera; //Primary Cam

public:
	enum Camera_Movement;

	SCamera(Scene* s);
	float radz(float deg);

	glm::mat4 getCamViewMatrix();
	glm::mat4 getSkyboxViewMatrix();
	glm::mat4 CameraToScreenMatrixx();
	float fov = 50.0f;

	const float MovementSpeed = 5.5f;
	float MouseSensitivity = 1.f;

	static SCamera* getSceneCamera();
	float camDepthBufFC;
private:
	float near_plane, far_plane;
	glm::mat4 view = glm::mat4(1.0f);
	bool primary_cam = true;
	
};
