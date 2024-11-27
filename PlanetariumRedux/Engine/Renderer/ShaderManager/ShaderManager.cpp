#include "ShaderManager.h"

#include "../../../Utils/shader.h"
#include <GL/gl3w.h>



SunsetShader::SunsetShader() 
{
	shader_program = CompileShader("Shaders/BasicShader/SimpleShader.vert", "Shaders/BasicShader/SimpleShader.frag");
	diffuse = Vector3f(0.5f, 0.2f, 1.0f);
	bump = 0.2f;
	specular = 0.6f;
	sheen = 1.0f; 
	opacity = 1.0f;
}

SunsetShader::SunsetShader(unsigned int compiled_shader)
{
	shader_program = compiled_shader;
	sheen = 0.2f;
	opacity = 0.45f;
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
	glUniform3f(glGetUniformLocation(shader_program, "material.v_diffuse"), material.diffuse.x, material.diffuse.y, material.diffuse.z);
	glUniform3f(glGetUniformLocation(shader_program, "material.v_specular"), material.specular.x, material.specular.y, material.specular.z);
	glUniform3f(glGetUniformLocation(shader_program, "material.v_bump"), material.bump.x, material.bump.y, material.bump.z);
	glUniform1f(glGetUniformLocation(shader_program, "material.shininess"), material.sheen);
	glUniform1f(glGetUniformLocation(shader_program, "material.opacity"), material.opacity);
}

void SunsetShader::setProperties(SunsetMaterial mat)
{
	glUniform3f(glGetUniformLocation(shader_program, "material.v_diffuse"), mat.diffuse.x, mat.diffuse.y, mat.diffuse.z);
	glUniform3f(glGetUniformLocation(shader_program, "material.v_specular"), mat.specular.x, mat.specular.y, mat.specular.z);
	glUniform3f(glGetUniformLocation(shader_program, "material.v_bump"), mat.bump.x, mat.bump.y, mat.bump.z);
	glUniform1f(glGetUniformLocation(shader_program, "material.shininess"), mat.sheen);
	glUniform1f(glGetUniformLocation(shader_program, "material.opacity"), mat.opacity);
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
	makeShader("Shaders/PlanetShader/PlanetShader.vert", "Shaders/PlanetShader/PlanetShader.frag");
	makeShader("Shaders/Shadow/shadow.vert", "Shaders/Shadow/shadow.frag");

	makeShader("Shaders/PhongLight/phong.vert", "Shaders/PhongLight/phong.frag");
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
