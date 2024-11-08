#pragma once

#include <vector>

class SunsetShader
{
	static std::vector<SunsetShader*> shader_list;
	const char* vertex;
	const char* fragment;

	unsigned int shader_program;

	float sheen;
	float opacity;

public:
	SunsetShader();
	SunsetShader(unsigned int compiled_shader);
	SunsetShader(const char* vertex, const char* fragment);

	void useShader();
	unsigned int getProgram();

	static SunsetShader* getSunsetShader(int index);
};

struct ShaderManager 
{

};