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
	localTransform->position = Vector3f(0, 9, 30);

}


void CameraComponent::Start()
{
	
}

void CameraComponent::Update()
{
	if (Input::OnKeyPressed(GLFW_KEY_V)) {

		if (Input::OnKeyPressed(GLFW_KEY_C))
		{
			localTransform->position = localTransform->position - localTransform->up() * 2.0f;
		}

		if (Input::OnKeyPressed(GLFW_KEY_SPACE))
		{
			localTransform->position = localTransform->position + localTransform->up() * 2.0f;
		}

		if (Input::OnKeyPressed(GLFW_KEY_A))
		{
			localTransform->position = localTransform->position - localTransform->right() * 2.0f;
		}

		if (Input::OnKeyPressed(GLFW_KEY_D))
		{
			localTransform->position = localTransform->position + localTransform->right() * 2.0f;
		}

		if (Input::OnKeyPressed(GLFW_KEY_W))
		{
			localTransform->position = localTransform->position + localTransform->forward() * 2.0f;
		}

		if (Input::OnKeyPressed(GLFW_KEY_S))
		{
			localTransform->position = localTransform->position - localTransform->forward() * 2.0f;
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

			localTransform->setEulerAngles(Vector3f(pitch, yaw, roll));

			printf("\n angles - x . %f , y . %f, z . %f\n",
				r_2_degrees(localTransform->getEulerAngles().x), r_2_degrees(localTransform->getEulerAngles().y), r_2_degrees(localTransform->getEulerAngles().z));

		}

		else 
		{
			localTransform->setEulerAngles(Vector3f(0,0,0));
		}
	}

	/*
	if (!(transform->position == Vector3f(0))) {
		for (int i = 0; i < EngineObject::getObjectListSize(); i++)
		{
			Transform* pos_to_zero = &EngineObject::getGlobalObjectIndex(i)->localTransform;
			pos_to_zero->position = pos_to_zero->position - former_position;
		}
		localTransform->position = localTransform->position - former_position;


		former_position = localTransform->position;
	}*/

}

void CameraComponent::FixedUpdate()
{
}

