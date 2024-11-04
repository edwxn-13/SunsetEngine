#include "Rigidbody.h"
#include "../../EngineObjects/EngineObject.h"
#include <glm/gtc/quaternion.hpp>

Rigidbody::Rigidbody(EngineObject * engineObject) : Component(engineObject)
{
	velocity = Vector3f(0);
	Start();
}

void Rigidbody::Start()
{
	useGravity = false;
	gravity = 2.4f;
}

void Rigidbody::addForce(Vector3f force)
{
	velocity = velocity + force;
}

void Rigidbody::addTorque(Vector3f t)
{
	torque = torque + t;
}

void Rigidbody::setTorque(Vector3f t)
{
	torque = t;
}

void Rigidbody::setVelocity(Vector3f v)
{
	velocity = v;
}

void Rigidbody::setGravityVector(Vector3f downVector)
{
	g_vector = downVector;
}

Vector3f Rigidbody::getVelocity()
{
	return velocity;
}

void Rigidbody::calculate_drag()
{

	if (useAngularDrag) 
	{
		angular_drag_force = (torque * torque.magnitude()) * 0.5f * air_density * angular_drag_coef;
	}

	else { angular_drag_force = 0; }

	if (useDrag) 
	{
		drag_force = (velocity * velocity.magnitude()) * 0.5 * air_density * drag_coef;
	}

	else { drag_force = 0; }
}

void Rigidbody::calculate_contact_force()
{

}

void Rigidbody::Update() 
{
	if (useGravity) 
	{
		velocity = velocity + (g_vector * gravity * 0.001f);
	}

	calculate_drag();
	if (torque.x != 0) 
	{
		drag_coef = drag_coef;
	}

	if (drag_force.magnitude() < velocity.magnitude()) 
	{
	}

	if (angular_drag_force.magnitude() < angular_drag_force.magnitude())
	{
	}
	velocity = velocity - drag_force;

	torque = torque - angular_drag_force;

	transform->Rotate(torque);
	transform->position = transform->position + velocity * 0.00001f;
}

void Rigidbody::FixedUpdate()
{

}

