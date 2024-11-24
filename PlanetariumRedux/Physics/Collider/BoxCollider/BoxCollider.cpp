#include "BoxCollider.h"
#include "../../../SunsetCore.h"
#include "../../../Components/Rigidbody/Rigidbody.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../../Camera/camera.h"

BoxCollider::BoxCollider(Vector3f pos, EngineObject* origin, float w, float h, float d) : Collider(pos, origin)
{
	width = w; height = h; depth = d;
}

bool BoxCollider::inCollision()
{
	if (originator_rigid)
		if (originator_rigid->getVelocity() == 0) {  }
	
	for (Collider* collider : collider_manager->collider_list)
	{		
		if((collider->transform.position - transform.position).magnitude() > 100.0f)
		{
			continue;
		}

		if (BoxCollider* col = (BoxCollider*)collider) { return AABB(col); }

	
	};
	return false;
}

float BoxCollider::min_x()
{
	return  (transform.position.x - (0.5f * width)) * transform.Scale.x;
}

float BoxCollider::max_x()
{
	return  (transform.position.x + (0.5f * width)) * transform.Scale.x;
}

float BoxCollider::min_y()
{
	return  (transform.position.y - (0.5f * depth)) * transform.Scale.y;
}

float BoxCollider::max_y()
{
	return  (transform.position.y + (0.5f * depth)) * transform.Scale.y;
}

float BoxCollider::min_z()
{
	return  (transform.position.z - (0.5f * height)) * transform.Scale.z;
}

float BoxCollider::max_z()
{
	return  (transform.position.z + (0.5f * height)) * transform.Scale.z;
}

bool BoxCollider::AABB(BoxCollider* col)
{
	if (min_x() <= col->max_x() &&
		max_x() >= col->min_x() &&
		min_y() <= col->max_y() &&
		max_y() >= col->min_y() &&
		min_z() <= col->max_z() &&
		max_z() >= col->min_z()) {
		return true;
	}
	return false;
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
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_CLAMP);
}


