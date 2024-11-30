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

	for (int i = 0; i < 9; i++)
	{
		planet_mesh.indecies = subdivide_sphere();
	}

	for (p_index tri : planet_mesh.indecies)
	{
		for (int i = 0; i < 3; i++) 
		{
			planet_mesh.r_indices.push_back(tri.t[i]);

			planet_mesh.vertices[tri.t[i]].normal = normalize(planet_mesh.vertices[tri.t[i]].point);

			p_vec3 uv;
			uv.x = .5f - atan2(planet_mesh.vertices[tri.t[i]].point.y, planet_mesh.vertices[tri.t[i]].point.x) / (2 * glm::pi<float>());
			uv.y = .5f - asin(planet_mesh.vertices[tri.t[i]].point.z / planet_mesh.vertices.size()) / glm::pi<float>();
			uv.z = 0;
			planet_mesh.vertices[tri.t[i]].tc = uv;

			//{ (atan2(planet_mesh.vertices[tri.t[i]].point.z, planet_mesh.vertices[tri.t[i]].point.x / (2.0f * glm::pi<float>()))) , (asin(planet_mesh.vertices[tri.t[i]].point.y) / glm::pi<float>()) + 0.5f };
		}
	}

	planet_mesh.GeneratePlanet();
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

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, FALSE, sizeof(p_vert), (void*)offsetof(p_vert, colour));

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

PlanetRenderer::PlanetRenderer(EngineObject* engineObject, PlanetSettings settings) : RenderingComponent(engineObject)
{
	planet_mesh.settings = settings;
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

p_vec3 minus(p_vec3 a, p_vec3 b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

p_vec3 multi(p_vec3 a, float b)
{
	return {a.x * b, a.y * b, a.z * b};
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
	p_vec3 mid = { (vertices[a].point.x + vertices[b].point.x) / 2,(vertices[a].point.y + vertices[b].point.y) / 2,(vertices[a].point.z + vertices[b].point.z) / 2 };
}

NFContainer::NFContainer(float rockiness)
{
	perlin = PerlinClass(8 , 2);
	for (int i = 0; i < layers; i++) 
	{
		noise[i] = SimplexNoise(rockiness, 1, 3.0f, 0.5f);
	}
	
}

float NFContainer::getFloat(p_vec3 v, PlanetSettings settings)
{
	float offset = 0.0f;
	float strength = settings.height;
	
	float min_value = settings.water_level;

	for (size_t i = 0; i < 9; i++) 
	{
		offset += (noise[2].fractal(i + 1,v.x, v.y, v.z) + 1.0f) * 0.5f;
	}
	offset = glm::max<float>(0, offset - min_value);
	return offset * strength;
}

p_vec3 NFContainer::CalcVert(p_vec3 v, PlanetSettings settings) 
{
	float offset = getFloat(v, settings);
	p_vec3 point = multi(v, (offset + 1) * settings.radius);
	return point;
}

void PlanetMesh::GeneratePlanet() 
{
	NFContainer nfc = NFContainer(settings.rockiness);
	
	for (p_vert& vert : vertices)
	{
		p_vec3 sample = vert.point;
		vert.point = nfc.CalcVert(vert.point, settings);
		float v_height = magnitude(vert.point);

		float altitude = (v_height - settings.radius)/settings.radius;

		vert.colour = settings.m_biome.calcBiome(altitude, 
			glm::abs(vert.normal.y) + (0.2 * nfc.noise[2].fractal(2, sample.z, sample.x, sample.y))  + (0.1 * nfc.noise[2].fractal(4,sample.z,sample.x, sample.y)) ,
			(nfc.noise[2].fractal(2,sample.x, sample.y, sample.z) + 1) * 0.5f);
	}
	//recalculate_normals();
}

void PlanetMesh::recalculate_normals()
{
	for (p_vert& vert : vertices) 
	{
		vert.normal = { 0,0,0 };
	}

	for (p_index index : indecies) 
	{		
		Vector3f a, b, c;
		a = Vector3f(vertices[index.t[0]].point.x, vertices[index.t[0]].point.y, vertices[index.t[0]].point.z);
		b = Vector3f(vertices[index.t[1]].point.x, vertices[index.t[1]].point.y, vertices[index.t[1]].point.z);
		c = Vector3f(vertices[index.t[2]].point.x, vertices[index.t[2]].point.y, vertices[index.t[2]].point.z);

		Vector3f p = glm::cross((b-a).glm(),(c-a).glm());

		vertices[index.t[0]].normal = add(vertices[index.t[0]].normal, {p.x,p.y,p.z});
		vertices[index.t[1]].normal = add(vertices[index.t[1]].normal, { p.x,p.y,p.z });
		vertices[index.t[2]].normal = add(vertices[index.t[2]].normal, { p.x,p.y,p.z });
	}

	for (p_vert& vert : vertices)
	{
		vert.normal = normalize(vert.normal);
	}
}

p_vec3 BiomeManager::calcBiome(float altitude, float lattitude, float perciptiation)
{
	//float thermal_energy = 29.0f + (float)(rand() % 29); //kelvin
	float a_value = (1- (1/((90 * altitude) + 1)));

	float p_value = glm::max<float>(0, perciptiation - 0.5);

	float l_value = 1 - lattitude;

	float temp = glm::max<float>(0,(l_value - (a_value * a_value)));

	float aridness = temp * glm::exp((1 - p_value));

	float temperature = (altitude); 

	p_vec3 biome_colour;

	if (a_value == 0) 
	{
		return { a_value,a_value,a_value };
	}

	return { temp,temp,temp };
}
