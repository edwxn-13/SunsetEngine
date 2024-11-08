#pragma once

class SunsetShader 
{
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
};

struct ShaderManager 
{

};