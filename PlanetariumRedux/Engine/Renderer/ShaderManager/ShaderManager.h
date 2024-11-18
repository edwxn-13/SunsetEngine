#pragma once

#include <vector>
#include "../../../Maths/MathHeader.h"

struct SunsetMaterial 
{
	Vector3f diffuse;
	Vector3f specular;
	Vector3f bump;
	float sheen;
	float opacity;

	float reflectivness;
	float emission;
	float ior;
};

class SunsetShader
{
	const char* vertex;
	const char* fragment;

	unsigned int shader_program;

	Vector3f diffuse;
	Vector3f specular;
	Vector3f bump;
	float sheen;
	float opacity;

	SunsetMaterial material;

	bool useTexture = true;

public:
	SunsetShader();
	SunsetShader(unsigned int compiled_shader);
	SunsetShader(const char* vertex, const char* fragment);

	void setProgram(unsigned int program);
	void setInt(const char* name , int value);
	void setFloat(const char* name, float value);
	void setVector(const char* name, Vector3f value);
	void setMat(const char* name, glm::mat4 value);
	void setProperties();

	void useShader();
	unsigned int getProgram();
};

struct ShaderManager 
{
	ShaderManager();
	std::vector<SunsetShader*> shader_list;
	void setupShaders();
	SunsetShader* getSunsetShader(int index);

	SunsetShader* shaderLookup(const char * id);
	SunsetShader * makeShader(const char* vertex, const char* fragment);
};