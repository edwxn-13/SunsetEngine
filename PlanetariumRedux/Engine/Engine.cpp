#include "Engine.h"
#include "Scene/Scene.h"
#include <GLFW/glfw3.h>
#include <iostream>

#define WIDTH 1024
#define HEIGHT 900

#define SH_MAP_WIDTH 2048
#define SH_MAP_HEIGHT 2048

Engine::Engine() : scene_manager()
{
	glfwInit();
	application_window = glfwCreateWindow(WIDTH, HEIGHT, "Sunset Engine v1.10", NULL, NULL);
	glfwMakeContextCurrent(application_window);
	renderer = Renderer(application_window);
}

void Engine::EngineStart()
{
	OnStart();
	EngineLoop();
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
		OnUpdate();
		OnFixedUpdate();
		PauseGame();
	}
}

void Engine::OnStart()
{
	scene_manager.active_scene->StartScene();
}

void Engine::OnFixedUpdate()
{
	scene_manager.active_scene->FixedUpdate();

}

void Engine::OnUpdate()
{
	scene_manager.active_scene->UpdateScene();
}

void Engine::PauseGame()
{

}


