#include "PrimativeObject.h"

#include "../../Components/PrimativeRenderer/PrimativeRenderer.h"

Plane::Plane(Scene* s, int pwidth, int plength) : EngineObject(s)
{
	width = pwidth; length = plength;
	PlaneRenderer* plane_renderer = new PlaneRenderer(this, width, length);
	addComponent(plane_renderer);
}

Cube::Cube(Scene* s) : EngineObject(s)
{

}

Sphere::Sphere(Scene* s) : EngineObject(s)
{

}
