#include "PlanetSettings.h"

p_vec3 BiomeManager::calcBiome(float altitude, float lattitude, float perciptiation, p_vec3 planet_type)
{
	//float thermal_energy = 29.0f + (float)(rand() % 29); //kelvin
	float a_value = (1 - (1 / ((90 * altitude) + 1)));

	float p_value = glm::max<float>(0, perciptiation - 0.5);

	float l_value = 1 - lattitude;

	float temp = glm::max<float>(0, (l_value - (a_value * a_value)));

	float aridness = temp * glm::exp((1 - p_value));

	if (a_value == 1)
	{
		return { a_value,a_value,a_value };
	}

	return { temp * planet_type.x ,p_value * planet_type.y ,a_value };
}
