#include "PlanetRenderer.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

using Lookup = std::map<std::pair<p_vec3, p_vec3>, p_vec3>;
PlanetRenderer::PlanetRenderer(EngineObject* engineObject) : RenderingComponent(engineObject)
{
	
}

void PlanetRenderer::loadMesh()
{
	planet_mesh.indecies = indecies;
	planet_mesh.vertices = vertices;

	for (int i = 0; i < 6; i++)
	{
		planet_mesh.indecies = subdivide_sphere();
	}

	for (p_index tri : planet_mesh.indecies)
	{
		planet_mesh.r_indices.push_back(tri.t[0]);
		planet_mesh.r_indices.push_back(tri.t[1]);
		planet_mesh.r_indices.push_back(tri.t[2]);

		planet_mesh.vertices[tri.t[0]].normal = normalize(planet_mesh.vertices[tri.t[0]].point);
		planet_mesh.vertices[tri.t[1]].normal = normalize(planet_mesh.vertices[tri.t[1]].point);
		planet_mesh.vertices[tri.t[2]].normal = normalize(planet_mesh.vertices[tri.t[2]].point);
	}
}

void PlanetRenderer::setUpMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, planet_mesh.vertices.size() * sizeof(p_vert), &planet_mesh.vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, planet_mesh.r_indices.size() * sizeof(unsigned int),
		&planet_mesh.r_indices[0], GL_STATIC_DRAW);

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
	glDrawElements(GL_TRIANGLES, planet_mesh.r_indices.size() , GL_UNSIGNED_INT, 0);
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

p_vec3 add(p_vec3 a, p_vec3 b) 
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}


std::vector<p_index> PlanetRenderer::subdivide_sphere()
{
	std::map<std::pair<unsigned int, unsigned int>, unsigned int> edge_lookup;
	std::vector<p_index> index;
	std::vector<p_vert> verts;
	for (p_index tri : planet_mesh.indecies)
	{
		std::array<unsigned int, 3> midpoints;

		for (int i = 0; i < 3; i++) 
		{
			midpoints[i] = subdivide_edge(edge_lookup, planet_mesh.vertices, tri.t[i], tri.t[(i+1)%3]);
		}
		index.push_back({ tri.t[0], midpoints[0], midpoints[2] });
		index.push_back({ tri.t[1], midpoints[1], midpoints[0] });
		index.push_back({ tri.t[2], midpoints[2], midpoints[1] });
		index.push_back({ midpoints[0], midpoints[1], midpoints[2] });

	}

	return index;
}

unsigned int PlanetRenderer::subdivide_edge(std::map<std::pair<unsigned int, unsigned int>, unsigned int>& lookup, std::vector<p_vert>& vertices, unsigned int a, unsigned int b)
{
	using hfunc = std::map<std::pair<unsigned int, unsigned int>, unsigned int>;
	hfunc::key_type key(a, b);
	if (key.first > key.second)
	{
		std::swap(key.first, key.second);
	}

	auto inserted = lookup.insert({ key, vertices.size() });
	if (inserted.second) 
	{
		auto& edge0 = vertices[a];
		auto& edge1 = vertices[b];
		auto point = normalize(add(edge0.point,  edge1.point));
		vertices.push_back({point, point});
	}

	return inserted.first->second;
	//p_vec3 mid = { (vertices[a].point.x + vertices[b].point.x) / 2,(vertices[a].point.y + vertices[b].point.y) / 2,(vertices[a].point.z + vertices[b].point.z) / 2 };
}