#include "../PlanetMath.h"

struct Biome
{
	p_vec3 biome_colour;
	float temperature;
	unsigned int terrain_texture;
};

struct BiomeManager
{
	float water_level;
	Biome biome_regions[5] =
	{
		{0.9f ,0.9f ,1.0f},
		{ 0.7f, 0.7f, 0.7f},
		{ 0.2f, 0.7f, 0.2f},
		{ 0.7f, 0.7f, 0.1f},
		{ 0.1f, 0.1f, 0.8f }
	};
	p_vec3 calcBiome(float altitude, float lattitude, float perciptiation, p_vec3 planet_type);
};

struct PlanetSettings
{
	BiomeManager m_biome;
	float radius = 600; //whatever -> in meteres
	float rockiness = 0.75f; //0.2 - 5
	float height = 0.01f; //0 - 1 
	float water_level = 3.0f;
	float atmosphere_density = 1.0f;

	float thermal = 1.0f;
	float dryness = 1.0f;

	int detail = 4;
};