

#include <GL/gl3w.h>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShipController.h"
#include "../../../EngineObjects/EngineObject.h"
#include "../../../Components/Rigidbody/Rigidbody.h"
 

ShipController::ShipController(EngineObject* engineObject) : Component (engineObject)
{
	input_controller = Input();
}

void ShipController::Start()
{
	ship_rigidbody = engineObject->getComponentOfType<Rigidbody>();
	ship_rigidbody->mass = 4000.0f;
	ship_stats = ShipStats();
}

void ShipController::Update(float deltaTime)
{
	input_controller.Update();
	float yaw, roll, pitch = 0.0f;

	printf("pos x: %f, y: %f, z: %f  - velocity : %f  -  DFO - %f\n", 
		transform->position.x, transform->position.y, transform->position.z,
		ship_rigidbody->getVelocity().magnitude(), transform->position.magnitude());
	//system("cls");


	roll = 0.0f;

	if (!input_controller.OnKeyPressed(GLFW_KEY_V)) {

		Thrust(deltaTime);

		if (input_controller.OnKeyPressed(GLFW_KEY_Q))
		{
			roll = -3.0f;
		}

		if (input_controller.OnKeyPressed(GLFW_KEY_E))
		{
			roll = 3.0f;
		}

		Vector2f inputAxis = input_controller.getMouseInputXY();

		yaw = input_controller.getMouseInputXY().x * 0.70f;
		pitch = input_controller.getMouseInputXY().y * 2.0f;

		Vector3f shipRotor = Vector3f(pitch, yaw, roll) * deltaTime * ship_stats.reaction_control_sys.rcs_torque;

		ship_rigidbody->addTorque(shipRotor);
	}
	
}

void ShipController::Thrust(float deltaTime)
{
	Vectoring(deltaTime);

	if (input_controller.OnKeyPressed(GLFW_KEY_W))
	{
		throttle += 0.1 * deltaTime;
	}

	if (input_controller.OnKeyPressed(GLFW_KEY_S))
	{
		throttle -= 0.1 * deltaTime;
	}

	if (throttle > max_throttle) { throttle = max_throttle; }
	if (throttle < -max_throttle/2) { throttle = -max_throttle/2; }

	ship_rigidbody->useDrag = inertial_dampners;
	ship_rigidbody->addForce(transform->forward() * throttle * ship_stats.thruster.specific_impulse );
}

void ShipController::Vectoring(float deltaTime)
{
	if (input_controller.OnKeyPressed(GLFW_KEY_C))
	{
		ship_rigidbody->addForce(transform->up() * -ship_stats.vector_thrust.vectoring_force * deltaTime);
	}

	if (input_controller.OnKeyPressed(GLFW_KEY_SPACE))
	{
		ship_rigidbody->addForce(transform->up() * ship_stats.vector_thrust.vectoring_force * deltaTime);
	}

	if (input_controller.OnKeyPressed(GLFW_KEY_A))
	{
		ship_rigidbody->addForce(transform->right() * -ship_stats.vector_thrust.vectoring_force * deltaTime);
	}

	if (input_controller.OnKeyPressed(GLFW_KEY_D))
	{
		ship_rigidbody->addForce(transform->right() * ship_stats.vector_thrust.vectoring_force * deltaTime);
	}
}

void ShipController::Stablizing(float deltaTime)
{
	if (!input_controller.OnKeyPressed(GLFW_KEY_C))
	{
		ship_rigidbody->addForce(transform->up() * ship_stats.vector_thrust.vectoring_force * deltaTime);
	}

	if (!input_controller.OnKeyPressed(GLFW_KEY_SPACE))
	{
		ship_rigidbody->addForce(transform->up() * ship_stats.vector_thrust.vectoring_force * deltaTime);
	}

	if (!input_controller.OnKeyPressed(GLFW_KEY_A))
	{
		ship_rigidbody->addForce(transform->right() * -ship_stats.vector_thrust.vectoring_force * deltaTime);
	}

	if (!input_controller.OnKeyPressed(GLFW_KEY_D))
	{
		ship_rigidbody->addForce(transform->right() * ship_stats.vector_thrust.vectoring_force * deltaTime);
	}
}

Vector3f ShipController::horizontal_velocity()
{
	return transform->right() * SunsetMath::Dot(transform->right(), ship_rigidbody->getVelocity());
}

Vector3f ShipController::vertical_velocity()
{
	return transform->up() * SunsetMath::Dot(transform->up(), ship_rigidbody->getVelocity());
}

void ShipController::FixedUpdate(float deltaTime)
{
	

}

void ShipController::ShipStats::ShipInit()
{
	ship_modules.push_back(&reactor);
	ship_modules.push_back(&vector_thrust);
	ship_modules.push_back(&thruster);
	ship_modules.push_back(&reaction_control_sys);

	thruster.specific_impulse = 36000;
	vector_thrust.vectoring_force = 12000;
	reaction_control_sys.rcs_torque = 12 * pow(10,3);
}
