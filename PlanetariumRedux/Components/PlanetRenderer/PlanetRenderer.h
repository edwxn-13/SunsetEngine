#pragma once
#include "../RenderingComponent.h"
#include "../../Engine/Renderer/ShaderManager/ShaderManager.h"
#include "../../Utils/ModelParser/ModelParser.h"
#include "../../Utils/noise.h"
#include "../../Maths/SimplexNoise.h"

#include <map>


struct Biome 
{
	float water_level = 1.01f;
	float rock_level;
	float ice_level;
};


struct p_vec3 { float x, y, z; };
struct p_vert { p_vec3 point, normal,tc,colour ; };
struct p_tri { p_vert v1, v2, v3; };
struct p_index { unsigned int t[3]; };

struct NFContainer
{
	int layers = 5;
	SimplexNoise noise[5];
	PerlinClass perlin;
	NFContainer();
	float getFloat(p_vec3 v);
	p_vec3 CalcVert(p_vec3 v);
};


p_vec3 normalize(p_vec3 vec);
float magnitude(p_vec3 vec);
p_vec3 add(p_vec3 a, p_vec3 b);
p_vec3 minus(p_vec3 a, p_vec3 b);

p_vec3 multi(p_vec3 a, float b);

struct PlanetMesh 
{
	std::vector<p_index> indecies;
	std::vector<p_vert> vertices;
	std::vector<unsigned int> r_indices;

	Biome biome;

	void GeneratePlanet();
	void recalculate_normals();

	void SerializeMesh();
	void DeserializeMesh();
	void PrintMesh();
};


class PlanetRenderer : public RenderingComponent 
{
public:
	PlanetRenderer(EngineObject * engineObject);
	void loadMesh() override;
	void setUpMesh() override;
	void renderMesh(unsigned int shader) override;

private:
	SunsetShader planet_shader;
	PlanetMesh planet_mesh;

	std::vector<p_index> subdivide_sphere();
	unsigned int subdivide_edge(std::map<std::pair<unsigned int, unsigned int>, unsigned int>& lookup, std::vector<p_vert>& vertices, unsigned int a, unsigned int b);

	unsigned int VBO, VAO, EBO;

	const float X = .525731112119133606f;
	const float Z = .850650808352039932f;
	const float N = 0.f;

	const std::vector<p_vert> vertices =
	{
	  {-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
	  {N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
	  {Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
	};

	const std::vector<p_index> indecies =
	{
	  {0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
	  {8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
	  {7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
	  {6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
	};

	std::vector<unsigned int> r_indices;

};