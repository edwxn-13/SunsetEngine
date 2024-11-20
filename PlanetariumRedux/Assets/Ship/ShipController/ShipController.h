#pragma once

#include "../../../Components/Component.h"
#include "../../../Engine/EngineUtils.h"
#include <string>

class Rigidbody;

struct ShipPart { std::string part_name; float mass; float cost; float power_draw; bool is_active = true; float integrity = 100.0f; };


struct Reactor : public ShipPart { };
struct Thruster : public ShipPart { float specific_impulse;};
struct VectoringThrust : public ShipPart { float vectoring_force; };
struct RCS : public ShipPart { float rcs_torque; };
struct FuelTank : public ShipPart { float total_fuel; float fuel; };

class ShipController : public Component 
{

public:
	ShipController(EngineObject * engineObject);

	void Start() override; 
	void FixedUpdate(float deltaTime) override;
	void Update(float deltaTime) override;
	void Thrust(float deltaTime);
	void Vectoring(float deltaTime);
	void Stablizing(float deltaTime);

	Vector3f horizontal_velocity();
	Vector3f vertical_velocity();

private: 

	struct ShipStats
	{

	private:
		std::vector<ShipPart*> ship_modules;
		void ShipInit();
		float ship_mass;


	public:

		ShipStats() { ShipInit(); }
		Thruster thruster;
		VectoringThrust vector_thrust;
		Reactor reactor;
		RCS reaction_control_sys;

	};
	Input input_controller;

	ShipStats ship_stats;

	float rcs_torque = 8.0f; 
	float thrust = 1020; 
	float vector_thrust = 1200;

	float throttle = 0.0f;
	float max_throttle = 1.0f;
	//float throttle_direction = 1.0f;

	bool inertial_dampners = true;
	
	Rigidbody* ship_rigidbody;
};