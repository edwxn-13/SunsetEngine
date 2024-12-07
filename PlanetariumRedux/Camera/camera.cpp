
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
	near_plane = .01f; far_plane = 30000000.0f;
	CameraComponent * cameraMovement = new CameraComponent(this, this);
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


	projection = glm::perspective(glm::radians(fov), (float)Screen::getScreenX()/ (float)Screen::getScreenY(), near_plane, far_plane);

	/*pos_mat = glm::translate(pos_mat, transform.position.glm());
	glm::mat4 rot = Quaternion::RotationMatrix(transform.rotation);
	pos_mat = pos_mat * rot;
	pos_mat = glm::scale(pos_mat, transform.scale.glm());
	view = glm::inverse(pos_mat);*/

	view = glm::inverse(transform.get_pos_mat());
	//view = glm::lookAt(transform.position.glm(), transform.position.glm() + transform.forward().glm(), transform.up().glm());

	glm::mat4 camMat = projection * view;
	return camMat;
}


glm::mat4 SCamera::getSkyboxViewMatrix()
{
	camDepthBufFC = 2.0 / (glm::log(far_plane + 1.0) / 0.69314718055994530941723212145818f);
	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(fov), (float)Screen::getScreenX() / (float)Screen::getScreenY(), .01f, 400000.f);
	glm::mat4 sky_view = glm::mat4(glm::mat3(view));

	glm::mat4 skyboxView = projection * sky_view;

	return skyboxView;
}

SCamera* SCamera::getSceneCamera()
{
	return p_camera;
}
	
