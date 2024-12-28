#pragma once

#include <vector>
#include "../../../Maths/MathHeader.h"

struct SunsetMaterial 
{
	Vector3f diffuse = 0.8f;
	Vector3f specular = 0.7f;
	Vector3f bump = 0.25f;
	float sheen = 1.0f;
	float opacity = 1;

	float reflectivness = 0.9f;
	float emission = 0.0f;
	float ior = 0.05f;
	float ambient = 1.0f;
};

class SunsetShader
{
	const char* vertex;
	const char* fragment;

	unsigned int shader_program;
	unsigned int geometry_program;

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

	void createGeometryShader(const char* name);

	void setProgram(unsigned int program);
	void setInt(const char* name , int value);
	void setFloat(const char* name, float value);
	void setVector(const char* name, Vector3f value);

	void setMat(const char* name, glm::mat4 value);
	void setProperties();
	void setProperties(SunsetMaterial mat);

	void setSun(Vector3f sunPos, float linear, float constant, float quadratic);

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