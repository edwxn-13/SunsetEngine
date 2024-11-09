#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Utils/error.h"
#include "Utils/shader.h"
#include "Utils/shadow.h"
#include "Camera/camera.h"
#include "Utils/texture.h"
#include "Components/MeshComponent/MeshComponent.h"
#include "Assets/MeshObject/MeshObject.h"
#include "Components/Rigidbody/Rigidbody.h"
#include "Engine/EngineUtils.h"
#include "Assets/Ship/ShipController/ShipController.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/ShaderManager/ShaderManager.h"
#include "Assets/Scenes/MainScene.h"
//light direction variable here


glm::vec3 lightDirection = glm::vec3(0.1f, -0.81f, -0.61f);
glm::vec3 lightPos = glm::vec3(2.f, 6.f, 7.f);



#define WIDTH 1024
#define HEIGHT 900

#define SH_MAP_WIDTH 2048
#define SH_MAP_HEIGHT 2048

void GlobalStartCall() 
{
	for (int i = 0; i < EngineObject::getObjectListSize(); i++)
	{
		EngineObject::getGlobalObjectIndex(i)->Start();
		EngineObject::getGlobalObjectIndex(i)->transform.Update(0);
	}
}

void fixedUpdateCaller() 
{
	bool call_fixed = true;

	if (call_fixed) 
	{
		for (int i = 0; i < EngineObject::getObjectListSize(); i++)
		{
			EngineObject::getGlobalObjectIndex(i)->FixedUpdate(12);
		}
	}
}

void preRenderSetUp() 
{
	for (int i = 0; i < EngineObject::getObjectListSize(); i++)
	{
		EngineObject::getGlobalObjectIndex(i)->Start();

		EngineObject::getGlobalObjectIndex(i)->transform.Update(0);
		if (MeshComponent* mesh = EngineObject::getGlobalObjectIndex(i)->getComponentOfType<MeshComponent>()) {
			if (mesh)
			{
				mesh->loadMesh();
				mesh->setUpMesh();
			}
		}
	}
}

void renderWithTexture(unsigned int shader, ShadowStruct shadow, glm::mat4 projectedLightSpaceMatrix, SCamera * Camera)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(shader);
	glUniform1i(glGetUniformLocation(shader, "Texture"), 0);

	glm::mat4 view = glm::mat4(1.f);
	view = Camera->getCamViewMatrix();

	glUniformMatrix4fv(glGetUniformLocation(shader, "camMat"), 1, GL_FALSE, glm::value_ptr(view));

	glUniform3f(glGetUniformLocation(shader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glUniform3f(glGetUniformLocation(shader, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
	glUniform3f(glGetUniformLocation(shader, "lightColour"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader, "camPos"), Camera->transform.position.x, Camera->transform.position.y, Camera->transform.position.z);
	
	for (int i = 0; i < EngineObject::getObjectListSize(); i++)
	{
		EngineObject::getGlobalObjectIndex(i)->Update(12);

		if (MeshComponent* mesh = EngineObject::getGlobalObjectIndex(i)->getComponentOfType<MeshComponent>()) {
			if (mesh)
			{
				mesh->renderMesh(shader);
			}
		}
	}
	
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE && action) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void SizeCallback(GLFWwindow* window, int w, int h)
{
	Screen::setScreenXY(w, h);
	glViewport(0, 0, w, h);
}

void processKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void generateDepthMap(unsigned int shadowShaderProgram, ShadowStruct shadow, glm::mat4 projectedLightSpaceMatrix, SCamera * Camera)
{
	glViewport(0, 0, SH_MAP_WIDTH, SH_MAP_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow.FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(projectedLightSpaceMatrix));
	renderWithTexture(shadowShaderProgram, shadow, projectedLightSpaceMatrix, Camera);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main(int argc, char** argv)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Sunset Engine v1.10", NULL, NULL);
	glfwMakeContextCurrent(window);

	Screen::setScreenXY(WIDTH, HEIGHT);
	Input::updateWindowValue(window);

	glfwSetWindowSizeCallback(window, SizeCallback);

	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);

	Renderer renderer = Renderer();
	renderer.app_window = window;
	renderer.shader_manager.getSunsetShader(0)->getProgram();

	ShadowStruct shadow = setup_shadowmap(SH_MAP_WIDTH, SH_MAP_HEIGHT);

	glDebugMessageCallback(DebguMessageCallback, 0);
	
	//SECTION A - EDIT THIS CODE TO TEST

	/*

	SCamera Camera = SCamera(new Scene());
	MeshObject gooch = MeshObject("objs/station/spaceStation.obj", new Scene());
	MeshObject ship = MeshObject("objs/fighter/fighter.obj", new Scene());

	ship.addComponent(new ShipController(&ship));
	gooch.transform.scale = Vector3f(22);
	gooch.localTransform.position = Vector3f(-200, 0, 0);
	ship.getTransform()->scale = Vector3f(5);
	ship.getTransform()->position = Vector3f(-50, -20.0f, 100.f);
	ship.addChild(&Camera);

	preRenderSetUp();

	Skybox skybox = Skybox();
	skybox.setupCubemap();*/
	

	MainScene scene = MainScene();
	scene.StartScene();
	renderer.setActiveCamera();
	renderer.preRenderSetUp(&scene);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float near_plane = 1.0f, far_plane = 70.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projectedLightSpaceMatrix = lightProjection * lightView;

		glViewport(0, 0, Screen::getScreenX(), Screen::getScreenY());

		Input::Update();
		renderer.RenderLoop(&scene, 0.4);

		glfwSwapBuffers(window);
		glfwPollEvents();
		processKeyboard(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}