#include "PlanetRenderer.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using Lookup = std::map<std::pair<p_vec3, p_vec3>, p_vec3>;
PlanetRenderer::PlanetRenderer(EngineObject* engineObject) : RenderingComponent(engineObject)
{
	
}

void PlanetRenderer::loadMesh()
{
	for (p_index tri : indecies) 
	{
		r_indices.push_back(tri.v1);
		r_indices.push_back(tri.v2);
		r_indices.push_back(tri.v3);

		vertices[tri.v1].normal = normalize(vertices[tri.v1].point);
		vertices[tri.v2].normal = normalize(vertices[tri.v2].point);
		vertices[tri.v3].normal = normalize(vertices[tri.v3].point);

	}
}

void PlanetRenderer::setUpMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(p_vert), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, r_indices.size() * sizeof(unsigned int),
		&r_indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(p_vert), (void *)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, FALSE, sizeof(p_vert), (void*)offsetof(p_vert,normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(p_vert), (void*)offsetof(p_vert, tc));

	glBindVertexArray(0);
}

void PlanetRenderer::renderMesh(unsigned int shader)
{
	planet_shader.setProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(planet_shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(transform->get_pos_mat()));
	planet_shader.setProperties();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, r_indices.size() , GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

p_vec3 normalize(p_vec3 vec)
{
	float mag = magnitude(vec);
	return {vec.x/mag, vec.y/mag, vec.z/mag};
}

float magnitude(p_vec3 vec)
{
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}
