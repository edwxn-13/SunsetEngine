#pragma once
#include "../Component.h"
#include "../../Maths/MathHeader.h"


class Rigidbody : public Component
{
public:
	Rigidbody(EngineObject* engineObject);

	void Start() override;
	void Update(float deltaTime) override;
	void FixedUpdate(float deltaTime) override;


	void addForce(Vector3f force);
	void addTorque(Vector3f t);
	void setTorque(Vector3f t);

	void setVelocity(Vector3f v);
	void setGravityVector(Vector3f downVector);
	
	Vector3f getVelocity();

private:

	void calculate_drag(float deltaTime);
	void calculate_contact_force();

	Vector3f g_vector = (0,0,-1);

	Vector3f velocity;
	Vector3f old_velocity;

	Vector3f acceleration;
	Vector3f torque;

	Vector3f drag_force;
	Vector3f angular_drag_force;

	float gravity = 9.8f;
	float angular_drag_coef = 27;

	float viscocity = 0.5;
	float drag_coef = 1.2;
	float air_density = 4.2f;

public:

	float mass = 1;

	bool useGravity = false;
	bool useDrag = true;
	bool useAngularDrag = true;
};

