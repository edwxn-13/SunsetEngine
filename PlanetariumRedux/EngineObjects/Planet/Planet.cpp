
#include "Planet.h"
#include "../../Components/PlanetRenderer/PlanetRenderer.h"

Planet::Planet(Scene* scene) : EngineObject(scene)
{
	planet_renderer = new PlanetRenderer(this);
	addComponent(planet_renderer);
}

