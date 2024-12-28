#pragma once

#include "SceneManager/SceneManager.h"
#include "Renderer/Renderer.h"
#include "Utility/Time.h"
#include <string>

struct GLFWwindow;

void OnSizeCallback(GLFWwindow* window, int w, int h);

class Engine
{
public:
	Engine(GLFWwindow* window);
	void EngineLoop();
	void OnStart();
	void OnFixedUpdate(float deltaTime);
	void OnUpdate(float deltaTime);
	void OnPhysicsUpdate();
	void PauseGame();
	void EngineStart();
	void EndEngine();
	void togglePause();
	void setPause(bool p);
	bool pauseState();


private:

	void processKeyboard(GLFWwindow* window);
	Time timer;
	GLFWwindow* application_window;
	SceneManager scene_manager;
	Renderer renderer;
	bool paused = false;
	int WindowHeight, WindowWidth = 500;

public:
	std::string application_name;

};

