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
	gravity = 0.28f;
}

void Rigidbody::addForce(Vector3f force)
{
	Vector3f delta_v = force / mass;
	velocity = velocity + delta_v;
}

void Rigidbody::addTorque(Vector3f t)
{
	Vector3f delta_w = t / mass;
	torque = torque + delta_w;
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

void Rigidbody::calculate_drag(float deltaTime)
{

	if (useAngularDrag) 
	{
		angular_drag_force = (torque * torque.magnitude()) * -0.5f * air_density * angular_drag_coef * deltaTime;
	}

	else { angular_drag_force = 0; }

	if (useDrag) 
	{
		float reynolds = air_density * viscocity * 20;

		drag_force = (velocity * velocity.magnitude()) * -0.5 * air_density * drag_coef * reynolds * deltaTime;
	}

	else { drag_force = 0; }

	addTorque(angular_drag_force);
	addForce(drag_force);
}

void Rigidbody::calculate_contact_force()
{

}

void Rigidbody::Update(float deltaTime)
{
	Vector3f current_v = velocity;
	acceleration = current_v - old_velocity;
	old_velocity = velocity;
}

void Rigidbody::FixedUpdate(float deltaTime)
{
	if (useGravity)
	{
		addForce(g_vector * gravity * mass * deltaTime);
	}

	calculate_drag(deltaTime);
	transform->Rotate(transform->right() * torque.x * deltaTime);
	transform->Rotate(transform->up() * torque.y * deltaTime);
	transform->Rotate(transform->forward() * torque.z * deltaTime);

	transform->position = transform->position + velocity * deltaTime;
}

