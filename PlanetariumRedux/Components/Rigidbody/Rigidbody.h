#pragma once
#include "../Component.h"
#include "../../Maths/MathHeader.h"


class Rigidbody : public Component
{
public:
	Rigidbody(EngineObject* engineObject);

	void Start() override;
	void Update() override;
	void FixedUpdate() override;


	void addForce(Vector3f force);
	void addTorque(Vector3f torque);

	
	Vector3f getVelocity();

private:

	Vector3f velocity;
	Vector3f torque;

	float mass = 1;
	float gravity = 9.8f;
	float angular_drag_coef = 0.3f;

public:

	bool useGravity = false;
	bool useDrag = true;
	bool useAngularDrag = true;
};

