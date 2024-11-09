#include "Engine.h"
#include "Scene/Scene.h"
#include "EngineUtils.h"
#include <GLFW/glfw3.h>
#include <iostream>

#define WIDTH 1024
#define HEIGHT 900

#define SH_MAP_WIDTH 2048
#define SH_MAP_HEIGHT 2048

Engine::Engine() : scene_manager(), paused(false)
{
	renderer.setUpShaders();
	glfwInit();
	application_window = glfwCreateWindow(WIDTH, HEIGHT, "Sunset Engine v1.10", NULL, NULL);

	glfwMakeContextCurrent(application_window);

	Screen::setScreenXY(WIDTH, HEIGHT);
	Input::updateWindowValue(application_window);

	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);

	renderer = Renderer(application_window);
	timer = Time();
}

void Engine::EngineStart()
{
	OnStart();
	EngineLoop();
}

void Engine::EndEngine()
{
	glfwDestroyWindow(application_window);
	glfwTerminate();
}

void Engine::togglePause()
{
	paused = !paused;
}

void Engine::setPause(bool p)
{
	paused = p;
}

bool Engine::pauseState()
{
	return paused;
}

void Engine::EngineLoop()
{
	renderer.preRenderSetUp(scene_manager.active_scene);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(application_window))
	{
		float deltaTime = timer.DeltaTime();
		OnUpdate(deltaTime);
		OnFixedUpdate(deltaTime);
		PauseGame();
		renderer.RenderLoop(scene_manager.active_scene, deltaTime);
		glfwSwapBuffers(application_window);
		glfwPollEvents();
		processKeyboard(application_window);
	}

	EndEngine();
}

void Engine::processKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Engine::OnStart()
{
	scene_manager.active_scene->StartScene();
}

void Engine::OnFixedUpdate(float deltaTime)
{
	timer.DeltaTime();

	bool call_fixed = timer.fixedUpdateChecker();

	if (call_fixed)
	{

	}
	
}

void Engine::OnUpdate(float deltaTime)
{
	glViewport(0, 0, Screen::getScreenX(), Screen::getScreenY());
	timer.updateTime();
	Input::Update();
	renderer.RenderLoop(scene_manager.active_scene, deltaTime);
}

void Engine::PauseGame()
{
	while (paused) 
	{

	}
}


