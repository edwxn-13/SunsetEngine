
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glcorearb.h>
#include "camera.h"
#include "../Components/CameraComponent/CameraComponent.h"
#include "../Engine/EngineUtils.h"
std::vector<SCamera*> SCamera::m_cam_list = {};

enum SCamera::Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

SCamera::SCamera()
{
	m_cam_list.push_back(this);
	InitCamera();
	CameraComponent * cameraMovement = new CameraComponent(this);
	addComponent(cameraMovement);
}

float SCamera::radz(float deg)
{
	float pi = glm::pi<float>();
	return deg * (pi / 180.0f);
}

void SCamera::InitCamera()
{
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Position = glm::vec3(0.0f, 0.0f, 0.0f);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);
	WorldUp = Up;
	Right = glm::normalize(glm::cross(Front, WorldUp));

	Yaw = -90.f;
	Pitch = 0.f;
}

void SCamera::LookAt(glm::vec3 target)
{
	//transform.Rotate(Vector3f(yoffset * MovementSpeed, 0 , xoffset * MovementSpeed));

	Front = glm::normalize(target - transform.position.glm());
	Right = glm::normalize(glm::cross(Front, Vector3f::Up.glm()));
	Up = glm::normalize(glm::cross(Right, Front));
	
}


void SCamera::Look()
{
	//transform.Rotate(Vector3f(-1.0f * (yoffset * MovementSpeed), 0, -1.0f *(xoffset * MovementSpeed)));
	Front = transform.forward().glm();
	Right = glm::normalize(glm::cross(Front, Vector3f::Up.glm()));
	Up = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 SCamera::getCamViewMatrix()
{
	view = glm::mat4(1.f);
	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(fov), (float)Screen::getScreenX()/ (float)Screen::getScreenY(), .01f, 10000.f);
	view = glm::lookAt(transform.position.glm(), transform.position.glm() + transform.forward().glm(), transform.up().glm());
	//view = glm::inverse(transform.get_pos_mat());

	glm::mat4 camMat = projection * view;

	return camMat;
}


glm::mat4 SCamera::getSkyboxViewMatrix()
{
	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(fov), (float)Screen::getScreenX() / (float)Screen::getScreenY(), .01f, 10000.f);
	//glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(transform.position.glm(), transform.position.glm() + transform.forward().glm(), transform.up().glm())));

	glm::mat4 sky_view = glm::mat4(glm::mat3(view));
	glm::mat4 camMat = projection * sky_view;

	return camMat;
}
	
