#include "ShaderManager.h"

#include "../../../Utils/shader.h"
#include <GL/gl3w.h>



SunsetShader::SunsetShader() 
{
	shader_program = CompileShader("Shaders/BasicShader/SimpleShader.vert", "Shaders/BasicShader/SimpleShader.frag");
	diffuse = 1.0f;
	bump = 0.2f;
	specular = 1.0f;
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

void SunsetShader::setProgram(unsigned int program)
{
	shader_program = program;
}

void SunsetShader::setInt(const char * name, int value)
{
	glUniform1i(glGetUniformLocation(shader_program, name), value);
}

void SunsetShader::setFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(shader_program, name), value);
}

void SunsetShader::setVector(const char* name, Vector3f value)
{
}

void SunsetShader::setMat(const char* name, glm::mat4 value)
{

}

void SunsetShader::setProperties()
{
	glUniform3f(glGetUniformLocation(shader_program, "material.v_diffuse"), diffuse.x, diffuse.y, diffuse.z );
	glUniform3f(glGetUniformLocation(shader_program, "material.v_specular"), specular.x, specular.y, specular.z);
	glUniform3f(glGetUniformLocation(shader_program, "material.v_bump"), bump.x, bump.y, bump.z);
	glUniform1f(glGetUniformLocation(shader_program, "material.shininess"), sheen);
	glUniform1f(glGetUniformLocation(shader_program, "material.opacity"), opacity);
}

void SunsetShader::useShader()
{
	glUseProgram(shader_program);
}

unsigned int SunsetShader::getProgram()
{
	return shader_program;
}

ShaderManager::ShaderManager()
{
	shader_list.reserve(20);
}

void ShaderManager::setupShaders()
{
	makeShader("Shaders/BasicShader/SimpleShader.vert", "Shaders/BasicShader/SimpleShader.frag");
	makeShader("Shaders/Skybox/skybox.vert", "Shaders/Skybox/skybox.frag");
	makeShader("Shaders/PhongLight/phong.vert", "Shaders/PhongLight/phong.frag");
	makeShader("Shaders/Shadow/shadow.vert", "Shaders/Shadow/shadow.frag");
	makeShader("Shaders/SimplePlanet/simplePlanetShader.vert", "Shaders/SimplePlanet/simplePlanetShader.frag");
}

SunsetShader* ShaderManager::getSunsetShader(int index)
{
	return shader_list[index];
}

SunsetShader* ShaderManager::shaderLookup(const char* id)
{
	return nullptr;
}

SunsetShader* ShaderManager::makeShader(const char* vertex, const char* fragment)
{
	SunsetShader* sunsetShader = new SunsetShader(vertex, fragment);
	shader_list.push_back(sunsetShader);
	return sunsetShader;
}
