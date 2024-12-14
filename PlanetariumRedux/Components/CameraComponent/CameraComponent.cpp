#include "CameraComponent.h"
#include <GL/gl3w.h>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Camera/camera.h"
#include "../../Engine/EngineUtils.h"

CameraComponent::CameraComponent(EngineObject* engineObject, SCamera * camera) : Component(engineObject)
{
	component_id = 23;
	transform->Translate(Vector3d(0, 20, 49));
	parent_cam = camera;
	input_controller = Input();
}

void CameraComponent::SphereIntersection()
{

}

void CameraComponent::Start()
{

}

void CameraComponent::Update(float deltaTime)
{
	input_controller.Update();

	if (input_controller.OnKeyPressed(GLFW_KEY_X)) 
	{
		parent_cam->fov = SunsetMath::Lerp(parent_cam->fov, 32, 0.2f);
	}

	else 
	{
		parent_cam->fov = SunsetMath::Lerp(parent_cam->fov, 45, 0.2f);
	}

	if (true) {

		if (input_controller.OnKeyPressed(GLFW_KEY_C))
		{
			transform->position = transform->position - transform->up() * move_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_SPACE))
		{
			transform->position = transform->position + transform->up() * move_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_A))
		{
			transform->position = transform->position - transform->right() * move_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_D))
		{
			transform->position = transform->position + transform->right() * move_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_W))
		{
			transform->position = transform->position + transform->forward() * move_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_S))
		{
			transform->position = transform->position - transform->forward() * move_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_Q))
		{
			roll = -1.0f * look_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_E))
		{
			roll = 1.0f * look_speed;
		}

		if (true)
		{
			Vector2f inputAxis = input_controller.getMouseInputXY();

			yaw = look_speed * input_controller.getMouseInputXY().x;
			pitch = look_speed * input_controller.getMouseInputXY().y;

			transform->Rotate(transform->right() * pitch * deltaTime);
			transform->Rotate(transform->up() * yaw * deltaTime);
			transform->Rotate(transform->forward() * -roll * deltaTime);
		}

		else 
		{
			transform->setEulerAngles(Vector3f(0,0,0));
		}

		roll = 0;
	}

}

void CameraComponent::FixedUpdate(float deltaTime)
{

}

