#pragma once


#include "../EngineObject.h"

class PlanetRenderer; 

struct Atmosphere
{
	float thickness;
	Vector3f gas_vector;
};

struct PlanetStats 
{
	bool has_atmosphere; 
	bool is_sun;

	Atmosphere atmospehre;
	float radius;
	float mass;
	float surface_temp;
};

class Planet : public EngineObject 
{
public:
	Planet(Scene* scene);

	PlanetStats stats;
	PlanetRenderer* planet_renderer;


private:

};