#include "ShaderManager.h"

#include "../../../Utils/shader.h"
#include <GL/gl3w.h>
#include <glm/gtc/type_ptr.hpp>
#include "../../../Utils/file.h"



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

void SunsetShader::createGeometryShader(const char* name)
{
	int success;
	char infolog[512];

	geometry_program = glCreateShader(GL_GEOMETRY_SHADER);
	char* geomSource = read_file(name);
	glShaderSource(geometry_program, 1, &geomSource, NULL);
	glCompileShader(geometry_program);

	glGetShaderiv(geometry_program, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(geometry_program, 512, NULL, infolog);
		fprintf(stderr, "geo problems - %s", infolog);
	}


	glAttachShader(shader_program, geometry_program);
	glDeleteShader(geometry_program);

	glLinkProgram(shader_program);
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
	glUniform3f(glGetUniformLocation(shader_program, name), value.x, value.y, value.z);
}

void SunsetShader::setMat(const char* name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, glm::value_ptr(value));
}

void SunsetShader::setProperties()
{
	glUniform3f(glGetUniformLocation(shader_program, "material.v_diffuse"), material.diffuse.x, material.diffuse.y, material.diffuse.z);
	glUniform3f(glGetUniformLocation(shader_program, "material.v_specular"), material.specular.x, material.specular.y, material.specular.z);
	glUniform3f(glGetUniformLocation(shader_program, "material.v_bump"), material.bump.x, material.bump.y, material.bump.z);
	glUniform3f(glGetUniformLocation(shader_program, "material.v_ambient"), material.bump.x, material.bump.y, material.bump.z);
	setVector("material.v_ambient", Vector3f(material.ambient));

	glUniform1f(glGetUniformLocation(shader_program, "material.shininess"), material.sheen);
	glUniform1f(glGetUniformLocation(shader_program, "material.opacity"), material.opacity);
}

void SunsetShader::setProperties(SunsetMaterial mat)
{
	glUniform3f(glGetUniformLocation(shader_program, "material.v_diffuse"), mat.diffuse.x, mat.diffuse.y, mat.diffuse.z);
	glUniform3f(glGetUniformLocation(shader_program, "material.v_specular"), mat.specular.x, mat.specular.y, mat.specular.z);
	glUniform3f(glGetUniformLocation(shader_program, "material.v_bump"), mat.bump.x, mat.bump.y, mat.bump.z);
	setVector("material.v_ambient", Vector3f(mat.ambient));

	glUniform1f(glGetUniformLocation(shader_program, "material.shininess"), mat.sheen);
	glUniform1f(glGetUniformLocation(shader_program, "material.opacity"), mat.opacity);
}

void SunsetShader::setSun(Vector3f sunPos, float linear, float constant, float quadratic)
{
	setVector("tSun.position", sunPos);

	setVector("tSun.ambient", Vector3f(1.0f));
	setVector("tSun.diffuse", Vector3f(1.0f));
	setVector("tSun.specular", Vector3f(1.0f));

	setFloat("tSun.constant", constant);
	setFloat("tSun.linear", linear);
	setFloat("tSun.quadratic", quadratic);
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
	shader_list[3]->createGeometryShader("Shaders/Shadow/shadow.gs");

	makeShader("Shaders/PlanetShader/AtmosphereShader.vert", "Shaders/PlanetShader/AtmosphereShader.frag");
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
