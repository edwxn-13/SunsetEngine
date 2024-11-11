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
#include "Engine/Engine.h"
#include "Engine/Renderer/ShaderManager/ShaderManager.h"
#include "Assets/Scenes/MainScene.h"
//light direction variable here

glm::vec3 lightDirection = glm::vec3(0.1f, -0.81f, -0.61f);
glm::vec3 lightPos = glm::vec3(2.f, 6.f, 7.f);

#define WIDTH 1024
#define HEIGHT 900

#define SH_MAP_WIDTH 2048
#define SH_MAP_HEIGHT 2048

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

void InitialSetup() 
{
	Time main_time = Time();

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

	MainScene scene = MainScene();
	scene.StartScene();
	renderer.preRenderSetUp(&scene);
	renderer.setActiveCamera();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		main_time.updateTime();
		float near_plane = 1.0f, far_plane = 70.5f;

		glViewport(0, 0, Screen::getScreenX(), Screen::getScreenY());

		Input::Update();
		renderer.RenderLoop(&scene, main_time.DeltaTime());

		glfwSwapBuffers(window);
		glfwPollEvents();
		processKeyboard(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

void EngineLaunch() 

{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Sunset Engine v1.10", NULL, NULL);
	glfwMakeContextCurrent(window);
	Screen::setScreenXY(WIDTH, HEIGHT);
	glfwSetWindowSizeCallback(window, SizeCallback);
	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);

	Engine engine = Engine(window);
	engine.EngineStart();
}

int main(int argc, char** argv)
{
	//InitialSetup();
	EngineLaunch();
	return 0;
}