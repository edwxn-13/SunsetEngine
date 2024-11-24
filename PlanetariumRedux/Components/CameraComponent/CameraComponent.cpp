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
	localTransform->Translate(Vector3f(20, 0, 0));
	parent_cam = camera;
	input_controller = Input();
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

	if (input_controller.OnKeyPressed(GLFW_KEY_V)) {

		if (input_controller.OnKeyPressed(GLFW_KEY_C))
		{
			localTransform->position = localTransform->position - localTransform->up() * move_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_SPACE))
		{
			localTransform->position = localTransform->position + localTransform->up() * move_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_A))
		{
			localTransform->position = localTransform->position - localTransform->right() * move_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_D))
		{
			localTransform->position = localTransform->position + localTransform->right() * move_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_W))
		{
			localTransform->position = localTransform->position + localTransform->forward() * move_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_S))
		{
			localTransform->position = localTransform->position - localTransform->forward() * move_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_Q))
		{
			roll += -1.0f * look_speed;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_E))
		{
			roll += 1.0f * look_speed;
		}

		if (true)
		{
			Vector2f inputAxis = input_controller.getMouseInputXY();

			yaw += look_speed * input_controller.getMouseInputXY().x;
			pitch += look_speed * input_controller.getMouseInputXY().y;

			localTransform->setEulerAngles(Vector3f(pitch, yaw, roll));
		}

		else 
		{
			localTransform->setEulerAngles(Vector3f(0,0,0));
		}
	}

}

void CameraComponent::FixedUpdate(float deltaTime)
{

}

