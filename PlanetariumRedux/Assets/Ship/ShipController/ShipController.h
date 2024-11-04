#pragma once

#include "../../../Components/Component.h"
class Rigidbody;

class ShipController : public Component 
{

public:
	ShipController(EngineObject * engineObject);

	void Start() override; 
	void FixedUpdate() override;
	void Update() override;

private: 

	float rcs_torque = 1; 
	float thrust = 5.0f; 
	float vector_thrust = 120.0f;
	
	Rigidbody* ship_rigidbody;
};