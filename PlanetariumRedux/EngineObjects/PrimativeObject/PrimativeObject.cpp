#include "PrimativeObject.h"

#include "../../Components/PrimativeRenderer/PrimativeRenderer.h"

Plane::Plane(Scene* s, float pwidth, float plength) : EngineObject(s)
{
	width = pwidth; length = plength;
	PlaneRenderer* plane_renderer = new PlaneRenderer(this, width, length);
	addComponent(plane_renderer);
}

Cube::Cube(Scene* s, float pwidth, float plength , float pdepth) : EngineObject(s)
{
	width = pwidth; length = plength; depth = pdepth;
	CubeRenderer* cube_renderer = new CubeRenderer(this, width, length, depth);
	addComponent(cube_renderer);
}

Sphere::Sphere(Scene* s) : EngineObject(s)
{

}
