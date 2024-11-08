#include "ShaderManager.h"

#include "../../../Utils/shader.h"
#include <GL/gl3w.h>

SunsetShader::SunsetShader() 
{
	sheen = 0.5; 
	opacity = 1.0f;
}

SunsetShader::SunsetShader(unsigned int compiled_shader)
{
	shader_program = compiled_shader;
	sheen = 0.5;
	opacity = 1.0f;
}

SunsetShader::SunsetShader(const char* vs, const char* fs)
{
	shader_program = CompileShader(vs, fs);
	vertex = vs; fragment = fs;
	sheen = 0.5;
	opacity = 1.0f;
}

void SunsetShader::useShader()
{
	glUseProgram(shader_program);
}

unsigned int SunsetShader::getProgram()
{
	return shader_program;
}
