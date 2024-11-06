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
	void addTorque(Vector3f t);
	void setTorque(Vector3f t);

	void setVelocity(Vector3f v);
	void setGravityVector(Vector3f downVector);
	
	Vector3f getVelocity();

private:

	void calculate_drag();	
	void calculate_contact_force();

	Vector3f g_vector;

	Vector3f velocity;
	Vector3f torque;

	Vector3f drag_force;
	Vector3f angular_drag_force;

	float gravity = 9.8f;
	float angular_drag_coef = 0.3f;
	float drag_coef = 0.3f;
	float air_density = 0.25f;

public:

	float mass = 1;

	bool useGravity = false;
	bool useDrag = true;
	bool useAngularDrag = true;
};

