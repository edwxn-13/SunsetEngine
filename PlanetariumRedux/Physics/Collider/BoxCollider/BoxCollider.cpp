#include "BoxCollider.h"
#include "../../../SunsetCore.h"
#include "../../../Components/Rigidbody/Rigidbody.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../../Camera/camera.h"

BoxCollider::BoxCollider(Vector3d pos, EngineObject* origin, float w, float h, float d) : Collider(pos, origin)
{
	
}

bool BoxCollider::inCollision()
{
	return true;
}

void BoxCollider::setUpDebugMesh()
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

void BoxCollider::renderDebugMesh()
{
	sunsetShader.useShader();

	glDisable(GL_DEPTH_CLAMP);
	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);

	

	glm::mat4 view = glm::mat4(1.f);
	view = SCamera::getSceneCamera()->getCamViewMatrix();
	glm::mat4 model = 1.f;

	sunsetShader.setProperties();
	glUniformMatrix4fv(glGetUniformLocation(sunsetShader.getProgram(), "camMat"), 1, GL_FALSE, glm::value_ptr(view));
	//glUniformMatrix4fv(glGetUniformLocation(sunsetShader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(transform->get_pos_mat()));

	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_CLAMP);
}


