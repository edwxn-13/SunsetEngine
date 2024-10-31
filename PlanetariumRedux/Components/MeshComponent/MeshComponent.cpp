#include "MeshComponent.h"
#include "../../EngineObjects/EngineObject.h"
#include "../../Utils/obj.h"
#include "../../Utils/texture.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

MeshComponent::MeshComponent(EngineObject* engineObject, const char* file, unsigned int shader) : Component(engineObject)
{
	component_id = 1;
	shaderProgram = shader;
	filename = file;
}

std::vector<Object> MeshComponent::getMesh() 
{
	return objs;
}

unsigned int MeshComponent::getType() 
{
	return shaderProgram;
}

void MeshComponent::loadMesh() 
{
	obj_parse(filename, &objs);
}

void MeshComponent::setUpMesh() 
{
	for (int i = 0; i < objs.size(); i++) {
		objs[i].texture = CreateTexture(objs[i].mtl.fil_name);
		objs[i].specular = CreateTexture(objs[i].mtl.spec_name);
		objs[i].bump = CreateTexture(objs[i].mtl.bump_name);

		glGenVertexArrays(1, &objs[i].VAO);
		glGenBuffers(1, &objs[i].VBO);
		glBindVertexArray(objs[i].VAO);
		glBindBuffer(GL_ARRAY_BUFFER, objs[i].VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (objs[i].tris.size() * 27), objs[i].tris.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MeshComponent::renderMesh() 
{
	for (int i = 0; i < objs.size(); i++)
	{
		glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), objs[i].texture);
		glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), objs[i].specular);
		glUniform1i(glGetUniformLocation(shaderProgram, "material.bump"), objs[i].bump);
		glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 49.0f);
		glUniform1f(glGetUniformLocation(shaderProgram, "opacity"), 1.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, objs[i].texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, objs[i].specular);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, objs[i].bump);

		glBindVertexArray(objs[i].VAO);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(engineObject->transform.get_pos_mat()));
		glDrawArrays(GL_TRIANGLES, 0, (objs[i].tris.size() * 3));
	}
}


