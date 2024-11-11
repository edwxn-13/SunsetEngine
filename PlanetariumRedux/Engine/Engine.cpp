#include "Engine.h"
#include "Scene/Scene.h"
#include "SceneManager/SceneManager.h"
#include "EngineUtils.h"
#include <GLFW/glfw3.h>
#include <iostream>
#define WIDTH 1024
#define HEIGHT 900


Engine::Engine(GLFWwindow* window) : scene_manager(0)
{
	application_window = window;
	renderer = Renderer(application_window);
	timer = Time();
	paused = false;
}

void Engine::EngineStart()
{
	OnStart();
	renderer.setActiveCamera();
	EngineLoop();
	EndEngine();
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
	while (!glfwWindowShouldClose(application_window))
	{
		float deltaTime = timer.DeltaTime();
		OnUpdate(deltaTime);

		glfwSwapBuffers(application_window);
		glfwPollEvents();
		processKeyboard(application_window);
	}
}

void Engine::processKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Engine::OnStart()
{
	scene_manager.active_scene->StartScene();
	renderer.preRenderSetUp(scene_manager.active_scene);
	glEnable(GL_DEPTH_TEST);
}

void Engine::OnFixedUpdate(float deltaTime)
{
	timer.DeltaTime();

	bool call_fixed = timer.fixedUpdateChecker();

	if (call_fixed)
	{
		int size = scene_manager.active_scene->SceneMembers.size();

		for (int i = 0; i < size; i++)
		{
			scene_manager.active_scene->FixedUpdate(deltaTime, i);
		}
		//scene_manager.active_scene->FixedUpdate(deltaTime);
	}
	
}

void Engine::OnUpdate(float deltaTime)
{
	timer.updateTime();
	glViewport(0, 0, Screen::getScreenX(), Screen::getScreenY());
	renderer.RenderLoop(scene_manager.active_scene, deltaTime);
}

void Engine::PauseGame()
{
	
}

void OnSizeCallback(GLFWwindow* window, int w, int h)
{
	Screen::setScreenXY(w, h);
	glViewport(0, 0, w, h);
}
