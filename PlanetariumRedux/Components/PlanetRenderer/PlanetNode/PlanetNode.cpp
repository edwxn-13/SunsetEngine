#include "PlanetNode.h"
#include "../PlanetRenderer.h"
#include "../../../Camera/camera.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

PlanetNode::PlanetNode()
{

}

PlanetNode* PlanetNode::getChild(int depth, int leaf)
{
	if (depth == 0) { return children[leaf]; }
	return nullptr;
	
}

void PlanetNode::setUpNode(const std::vector<p_vert>& verts)
{
	setUpVert(verts);

	glGenBuffers(1, &EBO);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, r_index.size() * sizeof(unsigned int), &r_index[0], GL_STATIC_DRAW);

	if (children[0] == nullptr) { return; }

	for (size_t i = 0; i < 4; i++)
	{
		children[i]->setUpNode(verts);
	}

}

void PlanetNode::setUpVert(const std::vector<p_vert>& verts)
{

	//positon = calc_node_pos(verts);

	int tri_index = 0;
	float total_height = 0;
	for (p_index triangle : triangles) 
	{
		for (size_t i = 0; i < 3; i++)
		{

			total_height += (verts[triangle.t[i]].alti / 3);
			vertex_list.push_back(verts[triangle.t[i]]);

			p_vec3 uv;
			uv.x = .5f - atan2(vertex_list[i].point.y, vertex_list[i].point.x) / (2 * glm::pi<float>());
			uv.y = .5f - asin(vertex_list[i].point.z / verts.size()) / glm::pi<float>();
			uv.z = 0;

			uv = { 0,0,0 };
			vertex_list[i].tc = uv;
			vertex_list[i].normal = normalize(vertex_list[i].point);

			r_index.push_back(triangle.t[i]);
		}

		tri_index++;
	}

	positon = multi(positon, total_height/tri_index);
}

void PlanetNode::renderChildren()
{

}

void PlanetNode::renderNode(SunsetShader& shader, const glm::mat4& transform_mat , unsigned int m_VAO, unsigned int m_VBO)
{
	glDrawElements(GL_TRIANGLES, r_index.size(), GL_UNSIGNED_INT, 0);
}

PlanetNode* PlanetNode::createNode()
{
	PlanetNode* temp = new PlanetNode();
	temp->parent = this;
	return temp;
}

PlanetNode** PlanetNode::split()
{
	return children;
}

p_vec3 PlanetNode::calc_node_pos(std::vector<p_vert> list)
{
	p_vec3 a = list[triangles[0].t[0]].point;
	p_vec3 b = list[triangles[0].t[1]].point;
	p_vec3 c = list[triangles[0].t[2]].point;

	return { (a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3, (a.z + b.z + c.z) / 3 };
}

std::vector<PlanetNode*> PlanetNode::getVisibleChildren(Vector3f camera_pos, const glm::mat4& transform_mat)
{
	glm::vec4 local_pos = glm::vec4(positon.x, positon.y, positon.z, 0);
	Vector3f position = glm::vec3(transform_mat * local_pos);
	float distance = SunsetMath::Magnitude(position - camera_pos);

	std::vector<PlanetNode*> renderGroup;

	if (children[1] == nullptr) 
	{
		renderGroup.push_back(this);
		return renderGroup;
	}

	//std::cout << "\ndistance from plate : " <<  distance << "\n\n";

	if (distance < 1000)
	{
		for (size_t i = 0; i < 4; i++)
		{
			std::vector<PlanetNode*> drawables = children[i]->getVisibleChildren(camera_pos, transform_mat);
			renderGroup.insert(renderGroup.end(), drawables.begin(), drawables.end());
		}
	}

	else 
	{
		renderGroup.push_back(this);
	}

	return renderGroup;
}

void NodeManager::renderNodes(SunsetShader& shader, const glm::mat4& transform_mat)
{
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glPolygonMode(GL_BACK, GL_LINE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(transform_mat));
	shader.setProperties();

	render_group = {};

	glBindVertexArray(masterVAO);

	for (int i = 0; i < 20; i++) 
	{
		std::vector<PlanetNode *> temp = root_node[i]->getVisibleChildren(camera->getRootPosition().glm(), transform_mat);
		render_group.insert(render_group.end(), temp.begin(), temp.end());
	}

	for (int i = 0; i < render_group.size(); i++) 
	{
		render_group[i]->renderNode(shader, transform_mat, masterVAO, masterVBO);
	}
	 
	glBindVertexArray(0);
}

void NodeManager::updateNodes()
{
	for (int i = 0; i < render_group.size(); i++)
	{
		
	}
}

void NodeManager::initManager()
{
	std::vector<PlanetNode*> t_renderList;
	t_renderList.reserve(400);
	for (int i = 0; i < 20; i++)
	{
		t_renderList.push_back(root_node[i]);
	}

	render_group = t_renderList;
}

void NodeManager::setupNodes(const std::vector<p_vert>& m_verts)
{

	verts = m_verts;

	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_CLAMP);

	glGenVertexArrays(1, &masterVAO);
	glBindVertexArray(masterVAO);

	glGenBuffers(1, &masterVBO);
	glBindBuffer(GL_ARRAY_BUFFER, masterVBO);

	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(p_vert), &verts[0], GL_STATIC_DRAW);

	for (int i = 0; i < 20; i++)
	{
		root_node[i]->setUpNode(verts);
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(p_vert), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, FALSE, sizeof(p_vert), (void*)offsetof(p_vert, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(p_vert), (void*)offsetof(p_vert, tc));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, FALSE, sizeof(p_vert), (void*)offsetof(p_vert, colour));

	glBindVertexArray(0);

}
