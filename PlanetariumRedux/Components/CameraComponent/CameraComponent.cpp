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
		transform->position = localTransform->position - transform->up() * 5.f;
	}

	if (Input::OnKeyPressed(GLFW_KEY_SPACE))
	{
		transform->position = localTransform->position + transform->up() * 5.f;
	}

	if (Input::OnKeyPressed(GLFW_KEY_A))
	{
		transform->position = localTransform->position - transform->right() * 5.f;
	}

	if (Input::OnKeyPressed(GLFW_KEY_D))
	{
		transform->position = localTransform->position + transform->right() * 5.f;
	}

	if (Input::OnKeyPressed(GLFW_KEY_W))
	{
		transform->position = localTransform->position + transform->forward() * 5.f;
	}

	if (Input::OnKeyPressed(GLFW_KEY_S))
	{
		transform->position = localTransform->position - transform->forward() * 5.f;
	}

	//roll = 0;

	if (Input::OnKeyPressed(GLFW_KEY_Q))
	{
		roll += -1.0f * 10.0f;
	}

	if (Input::OnKeyPressed(GLFW_KEY_E))
	{
		roll += 1.0f * 10.0f;
	}

	//Input::OnKeyPressed(GLFW_KEY_V)
	if (true)
	{
		Vector2f inputAxis = Input::getMouseInputXY();

		yaw += 10.0f * Input::getMouseInputXY().x;
		pitch += 10.0f * Input::getMouseInputXY().y;

		transform->setEulerAngles(Vector3f(pitch, yaw, roll));

		printf("\n angles - x . %f , y . %f, z . %f\n",
			r_2_degrees(transform->getEulerAngles().x), r_2_degrees(transform->getEulerAngles().y), r_2_degrees(transform->getEulerAngles().z));

	}


	/*for (int i = 0; i < EngineObject::getObjectListSize(); i++)
	{
		Transform* pos_to_zero = &EngineObject::getGlobalObjectIndex(i)->transform;
		pos_to_zero->position = pos_to_zero->position - transform->position;
	}
	transform->position = transform->position - transform->position;*/
}

void CameraComponent::FixedUpdate()
{
}

