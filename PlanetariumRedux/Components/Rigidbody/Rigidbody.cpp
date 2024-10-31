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

void Rigidbody::Update() 
{
	if (useGravity) 
	{
		velocity = velocity + (Vector3f::Down * gravity * 0.001f);
	}

	//addForce(transform->forward() * -0.01);

	//transform->Rotate(Vector3f(2, 0, 0));
	//Vector3f test = transform->forward() * -1;
	 
	//transform->position = transform->position + transform->forward() * - 3;

	//transform->position = transform->position + velocity;

}

void Rigidbody::FixedUpdate()
{

}

