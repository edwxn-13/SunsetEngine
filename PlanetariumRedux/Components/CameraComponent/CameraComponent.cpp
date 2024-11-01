#include "CameraComponent.h"
#include <GL/gl3w.h>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Camera/camera.h"
#include "../../Engine/EngineUtils.h"

CameraComponent::CameraComponent(EngineObject* engineObject) : Component(engineObject)
{
	component_id = 23;

}


void CameraComponent::Start()
{
	
}


void CameraComponent::Update()
{

	if (Input::OnKeyPressed(GLFW_KEY_C))
	{
		localTransform->position = localTransform->position - localTransform->up() * 5.f;
	}

	if (Input::OnKeyPressed(GLFW_KEY_SPACE))
	{
		localTransform->position = localTransform->position + localTransform->up() * 5.f;
	}

	if (Input::OnKeyPressed(GLFW_KEY_A))
	{
		localTransform->position = localTransform->position - localTransform->right() * 5.f;
	}

	if (Input::OnKeyPressed(GLFW_KEY_D))
	{
		localTransform->position = localTransform->position + localTransform->right() * 5.f;
	}

	if (Input::OnKeyPressed(GLFW_KEY_W))
	{
		localTransform->position = localTransform->position + localTransform->forward() * 5.f;
	}

	if (Input::OnKeyPressed(GLFW_KEY_S))
	{
		localTransform->position = localTransform->position - localTransform->forward() * 5.f;
	}

	roll = 0;

	if (Input::OnKeyPressed(GLFW_KEY_Q))
	{
		roll = -1.0f * 10.0f;
	}

	if (Input::OnKeyPressed(GLFW_KEY_E))
	{
		roll = 1.0f * 10.0f;
	}
	if (true)
	{
		Vector2f inputAxis = Input::getMouseInputXY();

		yaw = 10.0f * Input::getMouseInputXY().x;
		pitch = 10.0f * Input::getMouseInputXY().y;

		localTransform->Rotate(Vector3f(pitch, yaw, roll));

		printf("\n angles - x . %f , y . %f, z . %f\n",
			r_2_degrees(localTransform->getEulerAngles().x), r_2_degrees(localTransform->getEulerAngles().y), r_2_degrees(localTransform->getEulerAngles().z));

	}
}

void CameraComponent::FixedUpdate()
{
}

