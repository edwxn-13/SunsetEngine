#include "PrimativeRenderer.h"
#include "../../Camera/camera.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

float plane_vertices[] =
{
	//t1				
	//pos				//tex				//oo
	-0.9f, 0.9f, 0.f,	0.f, 1.f, 0.0f,		0.0f, 0.0f, 1.0f,	//tl
	0.9f, 0.9f, 0.f,	1.f, 1.f, 0.0f,		0.0f, 0.0f, 1.0f,	//tr
	0.9f, -0.9f, 0.f,	1.f, 0.f, 0.0f,		0.0f, 0.0f, 1.0f,	//br
	//t2					
	//pos					//tex
	-0.9f, 0.9f, 0.f,	0.0f, 0.0f, -1.0f,	0.f , 1.f,0.0f,	//tl
	0.9f, -0.9f, 0.f,	0.0f, 0.0f, -1.0f,	1.f, 0.f,0.0f,	//br
	-0.9f,-0.9f, 0.f,	0.0f, 0.0f, -1.0f,	0.f, 0.f,0.0f	//bl
};


PlaneRenderer::PlaneRenderer(EngineObject* engineObject, float& width, float& length) : RenderingComponent(engineObject)
{
	this->width = width;
	this->length = length;
}

void PlaneRenderer::loadMesh()
{
}

void PlaneRenderer::setUpMesh()
{
	/*glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);*/

	glCreateBuffers(1, &VBO);
	glNamedBufferStorage(VBO, sizeof(plane_vertices), plane_vertices, 0);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (9 * sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (9 * sizeof(float)), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, (9 * sizeof(float)), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void PlaneRenderer::renderMesh(unsigned int shader)
{
	sunsetShader.setProgram(shader);
	sunsetShader.useShader();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_CLAMP);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);


	glm::mat4 view = glm::mat4(1.f);
	view = SCamera::getSceneCamera()->getCamViewMatrix();
	glm::mat4 model = transform->get_pos_mat();

	sunsetShader.setProperties();
	glUniformMatrix4fv(glGetUniformLocation(shader, "camMat"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(transform->get_pos_mat()));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glDisable(GL_CULL_FACE);
}

CubeRenderer::CubeRenderer(EngineObject* engineObject, float& width, float& height, float& length) : RenderingComponent(engineObject)
{
	this->length = length;
	this->width = width;
	this->height = height;

	float new_vert[324] =
	{
		-width, -height, -length,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
		width, -height, -length,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
		width,  height, -length,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
		width,  height, -length,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
		-width, height, -length,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
		-width, -height, -length,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,

		//front face
		-width, -height,  length,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
		width, -height,  length,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
		width,  height,  length,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
		width,  height,  length,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
		-width,  height,  length,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
		-width, -height,  length,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,

		//left face
		-width, height,  length,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
		-width, height, -length,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
		-width, -height, -length,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
		-width, -height, -length,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
		-width, -height,  length,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
		-width,  height,  length,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,

		//right face
		width,  height,  length,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
		width,  height, -length,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
		width, -height, -length, 	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
		width, -height, -length,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
		width, -height,  length,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
		width,  height,  length,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,

		//bottom face
		-width, -height, -length,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
		width, -height, -length,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
		width, -height,  length,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
		width, -height,  length,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
		-width, -height,  length,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
		-width, -height, -length,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,

		//top face
		-width,  height, -length,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
		width,  height, -length,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
		width,  height,  length,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
		width,  height,  length,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
		-width,  height,  length,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
		-width,  height, -length, 	1.0f, 1.f, 1.0f, 	0.f,1.f,0.f
	};

	//cube_verts = new_vert;
}

void CubeRenderer::loadMesh()
{

}

void CubeRenderer::setUpMesh()
{


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verts), &cube_verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)3);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)6);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CubeRenderer::renderMesh(unsigned int shader)
{

	sunsetShader.setProgram(shader);
	sunsetShader.useShader();

	glDisable(GL_DEPTH_CLAMP);
	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);

	glm::mat4 view = glm::mat4(1.f);
	view = SCamera::getSceneCamera()->getCamViewMatrix();
	glm::mat4 model = transform->get_pos_mat();

	sunsetShader.setProperties();
	glUniformMatrix4fv(glGetUniformLocation(shader, "camMat"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(transform->get_pos_mat()));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_CLAMP);
}
