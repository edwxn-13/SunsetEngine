#include "PrimativeRenderer.h"
#include "../../Camera/camera.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

float plane_vertices[] =
{
	//t1				
	//pos					//tex
	-0.9f, 0.9f, 0.f,	0.f, 1.f,		//tl
	0.9f, 0.9f, 0.f,	1.f, 1.f,	//tr
	0.9f, -0.9f, 0.f,	1.f, 0.f,		//br
	//t2					
	//pos					//tex
	-0.9f, 0.9f, 0.f,	0.f , 1.f,	//tl
	0.9f,  -0.9f, 0.f,	1.f, 0.f,	//br
	-0.9f,  -0.9f, 0.f,	0.f, 0.f	//bl
};


Plane::Plane(EngineObject* engineObject) : RenderingComponent(engineObject)
{
	
}

void Plane::loadMesh()
{
}

void Plane::setUpMesh()
{
	glCreateBuffers(1, &VBO);
	glNamedBufferStorage(VBO, sizeof(plane_vertices), plane_vertices, 0);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (5 * sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (5 * sizeof(float)), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Plane::renderMesh(unsigned int shader)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_CLAMP);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	sunsetShader.useShader();

	glm::mat4 view = glm::mat4(1.f);
	view = SCamera::getSceneCamera()->getCamViewMatrix();

	glm::mat4 model = glm::mat4(5.f);

	model = glm::rotate(model, (float)glfwGetTime() / 2,glm::vec3(0.0f,1.0f,0.0f));
	glUniformMatrix4fv(glGetUniformLocation(sunsetShader.getProgram(), "camMat"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(sunsetShader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(transform->get_pos_mat()));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glDisable(GL_CULL_FACE);
}
