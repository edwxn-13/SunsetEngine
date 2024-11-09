
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glcorearb.h>
#include "camera.h"
#include "../Components/CameraComponent/CameraComponent.h"
#include "../Engine/EngineUtils.h"
#include "../Engine/Scene/Scene.h"
std::vector<SCamera*> SCamera::m_cam_list = {};
SCamera * SCamera::p_camera = nullptr;

enum SCamera::Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

SCamera::SCamera(Scene * s) : EngineObject(s)
{
	m_cam_list.push_back(this);
	CameraComponent * cameraMovement = new CameraComponent(this);
	addComponent(cameraMovement);
	if (!p_camera) { p_camera = this; }
}

float SCamera::radz(float deg)
{
	float pi = glm::pi<float>();
	return deg * (pi / 180.0f);
}

glm::mat4 SCamera::getCamViewMatrix()
{
	view = glm::mat4(1.f);
	glm::mat4 projection = glm::mat4(1.f);
	glm::mat4 pos_mat = glm::mat4(1.f);


	projection = glm::perspective(glm::radians(fov), (float)Screen::getScreenX()/ (float)Screen::getScreenY(), .01f, 10000.f);
	//view = glm::lookAt(transform.position.glm(), transform.position.glm() + transform.forward().glm(), transform.up().glm());

	pos_mat = glm::translate(pos_mat, -localTransform.position.glm());
	glm::mat4 rot = Quaternion::RotationMatrix(localTransform.rotation);
	pos_mat = pos_mat * rot;
	pos_mat = glm::scale(pos_mat, localTransform.scale.glm());

	//view = glm::inverse(pos_mat);

	view = glm::inverse(transform.get_pos_mat());
	glm::mat4 camMat = projection * view;
	return camMat;
}


glm::mat4 SCamera::getSkyboxViewMatrix()
{
	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(fov), (float)Screen::getScreenX() / (float)Screen::getScreenY(), .01f, 10000.f);
	glm::mat4 sky_view = glm::mat4(glm::mat3(view));
	glm::mat4 skyboxView = projection * sky_view;

	return skyboxView;
}

SCamera* SCamera::getSceneCamera()
{
	return p_camera;
}
	
