#pragma once

#include "../../Maths/MathHeader.h"

class EngineObject;

struct Collision 
{
	Vector3d position;
	Vector3f velocity;

	Vector3f normalised_velocity;

	float collision_distance;
	bool collision_found;

	EngineObject* engineObject;
};