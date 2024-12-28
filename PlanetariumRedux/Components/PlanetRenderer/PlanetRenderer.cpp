#include "PlanetRenderer.h"

#include "../../Camera/camera.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>


using Lookup = std::map<std::pair<p_vec3, p_vec3>, p_vec3>;
PlanetRenderer::PlanetRenderer(EngineObject* engineObject) : RenderingComponent(engineObject)
{
	planet_mat.ambient = 0.59f;
}

void PlanetRenderer::loadMesh()
{
	planet_mesh.indecies = indecies;
	planet_mesh.vertices = vertices;
	planet_mesh.setNodes();
	
}

void PlanetRenderer::setUpMesh()
{
	std::map<std::pair<unsigned int, unsigned int>, unsigned int> edge_lookup;
	for (int i = 0; i < 20; i++)
	{
		printf("loading percentage [%f] - please wait... \n\n", (float(i + 1) / 20.0f) * 100.0f);
		planet_mesh.manager.root_node[i] = subdivide_face(planet_mesh.manager.root_node[i], planet_mesh.settings.detail, 0, planet_mesh.settings.detail, edge_lookup);
	}
	planet_mesh.GeneratePlanet();
	planet_mesh.manager.setupNodes(planet_mesh.vertices);
	planet_mesh.manager.camera = SCamera::getSceneCamera();
}

void PlanetRenderer::renderMesh(unsigned int shader)
{
	planet_shader.setProgram(shader);
	planet_shader.setProperties(planet_mat);

	planet_mesh.manager.renderNodes(planet_shader, transform->get_pos_mat());
}

PlanetRenderer::PlanetRenderer(EngineObject* engineObject, PlanetSettings settings) : RenderingComponent(engineObject)
{
	planet_mesh.settings = settings;
}

PlanetNode * PlanetRenderer::subdivide_face(PlanetNode* root, int subd, int depth, int max_depth, std::map<std::pair<unsigned int, unsigned int>, unsigned int>& edge_lookup)
{
	std::vector<p_index> next_index;

	root->draw_distance = (planet_mesh.settings.radius * 2.0f * 3.14159265358979f) * (18.0f/360.0f) * 10.0f;

	p_vec3 init_a = planet_mesh.vertices[root->triangles[0].t[0]].point;
	p_vec3 init_b = planet_mesh.vertices[root->triangles[0].t[1]].point;
	p_vec3 init_c = planet_mesh.vertices[root->triangles[0].t[2]].point;

	p_vec3 sum_vec = add(add(init_a, init_b), init_c);
	root->positon = { sum_vec.x / 3, sum_vec.y / 3, sum_vec.z / 3 };
	root->positon = normalize(root->positon);

	std::array<unsigned int, 3> midpoints;

	root->active = depth;

	int tiledetail = 3;

	for (int detail_index = 0; detail_index < tiledetail; detail_index++) {

		std::vector<p_index> save_index;

		for (p_index index : root->triangles) {

			for (int i = 0; i < 3; i++)
			{
				midpoints[i] = subdivide_edge(edge_lookup, planet_mesh.vertices, index.t[i], index.t[(i + 1) % 3]);
			}

			p_index a = { index.t[0], midpoints[0], midpoints[2], subd, depth };
			p_index b = { index.t[1], midpoints[1], midpoints[0],  subd, depth };
			p_index c = { index.t[2], midpoints[2], midpoints[1],  subd , depth };
			p_index d = { midpoints[0], midpoints[1], midpoints[2], subd, depth };

			std::vector<p_index> temp_index_vector = { a,b,c,d };
			save_index.insert(save_index.end(),temp_index_vector.begin(), temp_index_vector.end());
			if (detail_index == 0)
			{
				next_index = temp_index_vector;
			}
		}
		root->triangles = save_index;

	}

	if (depth == max_depth)
	{
		return root;
	}

	for (size_t i = 0; i < 4; i++)
	{
		root->children[i] = new PlanetNode();
		root->children[i]->parent = root;

		root->children[i]->triangles.push_back(next_index[i]);
		root->children[i] = subdivide_face(root->children[i],subd, depth + 1, max_depth, edge_lookup);
	}

	return root;
}

void PlanetRenderer::renderAtmospehre(SunsetShader * atmospheric_shader)
{
	atmospheric_shader->setVector("planet.atmosphere_colour", planet_mesh.settings.atmosphere_gases);
	atmospheric_shader->setFloat("planet.radius", planet_mesh.settings.radius);
	atmospheric_shader->setFloat("planet.density", planet_mesh.settings.atmosphere_density);
	atmospheric_shader->setMat("model", transform->get_pos_mat());

	if (atmosphereVAO == 0) {

		float radius = planet_mesh.settings.radius;

		float vertices[] = {
		-radius, -radius, -radius,  0.0f,  0.0f, -1.0f,
		 radius, -radius, -radius,  0.0f,  0.0f, -1.0f,
		 radius,  radius, -radius,  0.0f,  0.0f, -1.0f,
		 radius,  radius, -radius,  0.0f,  0.0f, -1.0f,
		-radius,  radius, -radius,  0.0f,  0.0f, -1.0f,
		-radius, -radius, -radius,  0.0f,  0.0f, -1.0f,

		-radius, -radius,  radius,  0.0f,  0.0f,  1.0f,
		 radius, -radius,  radius,  0.0f,  0.0f,  1.0f,
		 radius,  radius,  radius,  0.0f,  0.0f,  1.0f,
		 radius,  radius,  radius,  0.0f,  0.0f,  1.0f,
		-radius,  radius,  radius,  0.0f,  0.0f,  1.0f,
		-radius, -radius,  radius,  0.0f,  0.0f,  1.0f,

		-radius,  radius,  radius, -1.0f,  0.0f,  0.0f,
		-radius,  radius, -radius, -1.0f,  0.0f,  0.0f,
		-radius, -radius, -radius, -1.0f,  0.0f,  0.0f,
		-radius, -radius, -radius, -1.0f,  0.0f,  0.0f,
		-radius, -radius,  radius, -1.0f,  0.0f,  0.0f,
		-radius,  radius,  radius, -1.0f,  0.0f,  0.0f,

		 radius,  radius,  radius,  1.0f,  0.0f,  0.0f,
		 radius,  radius, -radius,  1.0f,  0.0f,  0.0f,
		 radius, -radius, -radius,  1.0f,  0.0f,  0.0f,
		 radius, -radius, -radius,  1.0f,  0.0f,  0.0f,
		 radius, -radius,  radius,  1.0f,  0.0f,  0.0f,
		 radius,  radius,  radius,  1.0f,  0.0f,  0.0f,

		-radius, -radius, -radius,  0.0f, -1.0f,  0.0f,
		 radius, -radius, -radius,  0.0f, -1.0f,  0.0f,
		 radius, -radius,  radius,  0.0f, -1.0f,  0.0f,
		 radius, -radius,  radius,  0.0f, -1.0f,  0.0f,
		-radius, -radius,  radius,  0.0f, -1.0f,  0.0f,
		-radius, -radius, -radius,  0.0f, -1.0f,  0.0f,

		-radius,  radius, -radius,  0.0f,  1.0f,  0.0f,
		 radius,  radius, -radius,  0.0f,  1.0f,  0.0f,
		 radius,  radius,  radius,  0.0f,  1.0f,  0.0f,
		 radius,  radius,  radius,  0.0f,  1.0f,  0.0f,
		-radius,  radius,  radius,  0.0f,  1.0f,  0.0f,
		-radius,  radius, -radius,  0.0f,  1.0f,  0.0f
		};

		glGenVertexArrays(1, &atmosphereVAO);
		glBindVertexArray(atmosphereVAO);

		glGenBuffers(1, &atmosphereVBO);
		glBindBuffer(GL_ARRAY_BUFFER, atmosphereVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		glBindVertexArray(0);

	}

	//glCullFace(GL_BACK);

	glDisable(GL_CULL_FACE);
	//glBindVertexArray(atmosphereVAO);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glBindVertexArray(0);
	glEnable(GL_CULL_FACE);

	//glCullFace(GL_FRONT);

}

std::vector<p_index> PlanetRenderer::subdivide_sphere(int subd)
{
	std::map<std::pair<unsigned int, unsigned int>, unsigned int> edge_lookup;
	std::vector<p_index> index;

	int node_index = 0;
	for (p_index tri : planet_mesh.indecies)
	{	
		int upper_triangle_index = node_index % 4;

		//PlanetNode * node = planet_mesh.manager.root_node[tri.id - 1].getChild(subd,tri.id);

		std::array<unsigned int, 3> midpoints;

		for (int i = 0; i < 3; i++) 
		{
			midpoints[i] = subdivide_edge(edge_lookup, planet_mesh.vertices, tri.t[i], tri.t[(i+1)%3]);
		}

		p_index a = { tri.t[0], midpoints[0], midpoints[2], tri.id, upper_triangle_index };
		p_index b = { tri.t[1], midpoints[1], midpoints[0], tri.id, upper_triangle_index };
		p_index c = { tri.t[2], midpoints[2], midpoints[1], tri.id , upper_triangle_index };
		p_index d = { midpoints[0], midpoints[1], midpoints[2], tri.id, upper_triangle_index };

		index.push_back(a);
		index.push_back(b);
		index.push_back(c);
		index.push_back(d);

		//planet_mesh.manager.root_node->children[upper_triangle_index]->r_indices.push_back(a.t[]);

		node_index++;
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
	for (int i = 0; i < layers; i++) 
	{
		noise[i] = SimplexNoise(rockiness * (pow(2,i)), 0.5, 3.0f, 0.5f);
	}
	
	noise[0] = SimplexNoise(rockiness, 1.0f, 2.0f, 0.69f);
	noise[1] = SimplexNoise(rockiness * 2, 1.0f, 3.0f, 0.5f);
	noise[2] = SimplexNoise(rockiness * 1.4, 1.0f, 3.0f, 0.5f);

}

float NFContainer::getFloat(p_vec3 v, PlanetSettings settings)
{
	float offset = 0.0f;
	float strength = settings.height;
	
	float min_value = settings.water_level;

	for (size_t i = 0; i < 12; i++) 
	{
		float temp_offset = (noise[0].fractal(i + 1,v.x, v.y, v.z) + 1.0f) * 0.5f;
		offset += temp_offset;
	}

	offset = glm::max<float>(0, offset - (1.3 * min_value));

	return offset * strength;
}

float NFContainer::getDetailFloat(p_vec3 v, PlanetSettings settings)
{
	float offset = 0.0f;
	float strength = settings.height;


	for (size_t i = 0; i < 12; i++)
	{
		float temp_offset = (noise[1].fractal(i + 1, v.x, v.y, v.z) + 1.0f) * 0.5f;
		offset += temp_offset;
	}

	return offset * strength;
}


p_vec3 NFContainer::CalcVert(p_vec3 v, PlanetSettings settings) 
{
	float offset = getFloat(v, settings);
	float scale_fac = 1;
	scale_fac += getDetailFloat(v, settings);

	if (offset != 0) 
	{
		
	}

	float steepness = (noise[3].noise(v.x,v.y,v.z) + 1.0f) * 0.5f;

	p_vec3 point = multi(v, exp(offset * pow(scale_fac, steepness)) * settings.radius);
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
		vert.alti = v_height;
		float altitude = (v_height - settings.radius)/settings.radius;
		vert.colour = settings.m_biome.calcBiome(altitude, 
			glm::abs(vert.normal.y) + (0.2 * nfc.noise[2].fractal(2, sample.z, sample.x, sample.y))  + (0.1 * nfc.noise[2].fractal(4,sample.z,sample.x, sample.y)) ,
			(nfc.noise[2].fractal(2,sample.x, sample.y, sample.z) + 1) * 0.5f, 
			{ settings.thermal, settings.thermal, 0 });
	}
	//recalculate_normals();
}

void PlanetMesh::setNodes()
{
	for (int i = 0; i < indecies.size(); i++)
	{
		indecies[i].p_id = 0;
		indecies[i].id = i + 1;

		manager.root_node[i] = new PlanetNode();
		manager.root_node[i]->triangles.push_back(indecies[i]);
	}
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
