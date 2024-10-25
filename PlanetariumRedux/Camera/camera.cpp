
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "../Components/CameraComponent/CameraComponent.h"

enum SCamera::Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

SCamera::SCamera()
{
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

void SCamera::LookAt(glm::vec3 target, float distance, float xoffset, float yoffset)
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
	return glm::lookAt(transform.position.glm(), transform.position.glm() + transform.forward().glm(), transform.up().glm());
}


glm::mat4 SCamera::getSkyboxViewMatrix()
{
	return glm::mat4(glm::mat3 (glm::lookAt(transform.position.glm(), transform.position.glm() + transform.forward().glm(), transform.up().glm())));
}
	
