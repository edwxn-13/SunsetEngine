#pragma once
#include "../PlanetMath.h"
#include <vector>
#include <list>

class SCamera;
class SunsetShader;
class Transform;

struct PlanetNode
{
	PlanetNode();
	PlanetNode * getChild(int depth, int leaf);

	void setUpNode(const std::vector<p_vert>& verts);
	void setUpVert(const std::vector<p_vert> &verts);

	void renderChildren();
	void renderNode(SunsetShader& shader, const glm::mat4& transform_mat, unsigned int m_VAO, unsigned int m_VBO);

	PlanetNode* createNode();

	unsigned int VAO, VBO, EBO;

	PlanetNode** split();
	void join();
	p_vec3 calc_node_pos(std::vector<p_vert> list);
	std::vector<PlanetNode*> getVisibleChildren(Vector3f camera_pos, const glm::mat4 &transform_mat);
	p_vec3 positon;

	unsigned int t[3];

	p_vert vertices[3];
	std::vector<p_index> triangles;
	std::vector<p_vert> vertex_list;
	std::vector<unsigned int> r_index;

	PlanetNode* parent;
	PlanetNode* children[4] = {nullptr, nullptr, nullptr, nullptr};
	int active = 0;

	const float draw_distance = 500.0f;
};

struct PlanetRenderChunk
{
	std::vector<PlanetNode*> render_group;
};

struct NodeManager
{
	std::vector<PlanetNode*> render_group;
	std::vector<p_vert> verts;
	std::vector<unsigned int> final_indecies;

	glm::mat4 transform_mat;

	void renderNodes(SunsetShader& shader, const glm::mat4& transform_mat);
	void setupNodes(const std::vector<p_vert>& m_verts);
	void initManager();
	void updateNodes();

	PlanetNode * root_node[20];
	SCamera* camera;

	unsigned int masterVAO, masterVBO, masterEBO;

	float draw_distance = 200.0f;

	int max_nodes = 200;
	int min_depth = 0;
	int max_depth = 2;
};


std::vector<unsigned int> p_index_list(p_index a);
