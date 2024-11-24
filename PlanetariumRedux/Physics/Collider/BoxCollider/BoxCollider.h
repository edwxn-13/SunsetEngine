#pragma once

#include "../ColliderManager.h"

class SunsetShader;



class BoxCollider : public Collider 
{
public:
	BoxCollider(Vector3f pos, EngineObject * origin, float w, float h, float d);
	bool inCollision() override;

	float min_x();
	float max_x();

	float min_y();
	float max_y();

	float min_z();
	float max_z();

	bool AABB(BoxCollider* col);
	bool AASphere(BoxCollider* col);
	bool AATriangle(BoxCollider* col);

	void setUpDebugMesh() override;
	void renderDebugMesh() override;

private:
	float height, width, depth;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	SunsetShader sunsetShader;

	float cube_verts[324] =
	{
	-width, -height, -depth,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	width, -height, -depth,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	width,  height, -depth,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	width,  height, -depth,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	-width, height, -depth,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	-width, -height, -depth,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,

	//front face
	-width, -height,  depth,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	width, -height,  depth,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	width,  height,  depth,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	width,  height,  depth,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	-width,  height,  depth,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	-width, -height,  depth,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,

	//left face
	-width, height,  depth,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-width, height, -depth,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-width, -height, -depth,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-width, -height, -depth,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-width, -height,  depth,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-width,  height,  depth,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,

	//right face
	width,  height,  depth,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	width,  height, -depth,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	width, -height, -depth, 	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	width, -height, -depth,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	width, -height,  depth,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	width,  height,  depth,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,

	//bottom face
	-width, -height, -depth,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	width, -height, -depth,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	width, -height,  depth,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	width, -height,  depth,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	-width, -height,  depth,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	-width, -height, -depth,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,

	//top face
	-width,  height, -depth,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	width,  height, -depth,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	width,  height,  depth,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	width,  height,  depth,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	-width,  height,  depth,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	-width,  height, -depth, 	1.0f, 1.f, 1.0f, 	0.f,1.f,0.f
	};

};