#include "MeshRenderer.h"
#include <GL/glcorearb.h>
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "../../EngineObjects/EngineObject.h"

MeshRenderer::MeshRenderer(EngineObject* engineObject, const char* file) : RenderingComponent(engineObject), model(file)
{
	filename = file;
	shader = SunsetShader();
	
}


unsigned int MeshRenderer::getType()
{
	return shader.getProgram();
}

void MeshRenderer::loadMesh()
{

}

void MeshRenderer::setUpMesh()
{
}

void MeshRenderer::renderMesh(unsigned int shader)
{
	this->shader.setProgram(shader);
	model.Draw(this->shader , engineObject->transform.get_pos_mat());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}