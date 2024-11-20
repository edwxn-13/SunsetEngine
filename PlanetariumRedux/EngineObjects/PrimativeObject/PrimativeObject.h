#include "../EngineObject.h"

class Plane : public EngineObject 
{
public:
	Plane(Scene * s, float pwidth = 10, float plength = 10);
	float length, width = 10.0f;
};

class Cube : public EngineObject
{
public:
	Cube(Scene* s, float pwidth = 10, float plength = 10, float pdepth = 10.0f);
	float length = 10.0f; float width = 10.0f; float depth = 10.0f;
};

class Sphere : public EngineObject
{
public:
	Sphere(Scene* s);
	float length, width = 10.0f;
};

