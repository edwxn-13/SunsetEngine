

#include <GL/gl3w.h>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShipController.h"
#include "../../../EngineObjects/EngineObject.h"
#include "../../../Components/Rigidbody/Rigidbody.h"
#include "../../../Engine/EngineUtils.h"




ShipController::ShipController(EngineObject* engineObject) : Component (engineObject)
{
	
}

void ShipController::Start()
{
	ship_rigidbody = engineObject->getComponentOfType<Rigidbody>();
	ship_rigidbody->mass = 2.0f;
	ship_stats = ShipStats();
}

void ShipController::Update() 
{

	float yaw, roll, pitch = 0.0f;

	roll = 0.0f;

	if (!Input::OnKeyPressed(GLFW_KEY_V)) {

		if (Input::OnKeyPressed(GLFW_KEY_C))
		{
			ship_rigidbody->addForce(transform->up() * -vector_thrust * Time::DeltaTime());
		}

		if (Input::OnKeyPressed(GLFW_KEY_SPACE))
		{
			ship_rigidbody->addForce(transform->up() * vector_thrust * Time::DeltaTime());
		}

		if (Input::OnKeyPressed(GLFW_KEY_A))
		{
			ship_rigidbody->addForce(transform->right() * -vector_thrust * Time::DeltaTime());
		}

		if (Input::OnKeyPressed(GLFW_KEY_D))
		{
			ship_rigidbody->addForce(transform->right() * vector_thrust * Time::DeltaTime());
		}

		if (Input::OnKeyPressed(GLFW_KEY_W))
		{
			ship_rigidbody->addForce(transform->forward() * thrust * Time::DeltaTime());
		}

		if (Input::OnKeyPressed(GLFW_KEY_S))
		{
			ship_rigidbody->addForce(transform->forward() * -thrust * Time::DeltaTime());
		}

		if (Input::OnKeyPressed(GLFW_KEY_Q))
		{
			roll = -1.0f * rcs_torque;
		}

		if (Input::OnKeyPressed(GLFW_KEY_E))
		{
			roll = 1.0f * rcs_torque;
		}

		Vector2f inputAxis = Input::getMouseInputXY();

		yaw = rcs_torque * Input::getMouseInputXY().x * 0.70f;
		pitch = rcs_torque * Input::getMouseInputXY().y;

		Vector3f shipRotor = Vector3f(pitch, yaw, roll) * Time::DeltaTime();

		ship_rigidbody->addTorque(shipRotor);
	}
	
}

void ShipController::Thrust()
{
	if (Input::OnKeyPressed(GLFW_KEY_W))
	{
		throttle += 0.1 * Time::DeltaTime();
	}

	if (Input::OnKeyPressed(GLFW_KEY_S))
	{
		throttle -= 0.1 * Time::DeltaTime();
	}

	if (throttle > 100.0f) { throttle = 100.0f; }
	if (throttle < -50.0f) { throttle = -50.0f; }

	if (Input::OnKeyPressed(GLFW_KEY_A))
	{
		ship_rigidbody->addForce(transform->right() * -vector_thrust * Time::DeltaTime());
	}

	if (Input::OnKeyPressed(GLFW_KEY_D))
	{
		ship_rigidbody->addForce(transform->right() * vector_thrust * Time::DeltaTime());
	}

	ship_rigidbody->useDrag = inertial_dampners;
	ship_rigidbody->addForce(transform->forward() * throttle * ship_stats.thruster.specific_impulse * Time::DeltaTime());
}

void ShipController::FixedUpdate()
{

}

void ShipController::ShipStats::ShipInit()
{
	ship_modules.push_back(&reactor);
	ship_modules.push_back(&vector_thrust);
	ship_modules.push_back(&thruster);
	ship_modules.push_back(&reaction_control_sys);
}
