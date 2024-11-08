#pragma once

#include "SceneManager/SceneManager.h"
#include "Renderer/Renderer.h"

#include <string>

class GLFWwindow;

class Engine
{
public:
	Engine();

	void EngineLoop();
	void OnStart();
	void OnFixedUpdate();
	void OnUpdate();
	void PauseGame();
	void EngineStart();

	void togglePause();
	void setPause(bool p);
	bool pauseState();

private:
	GLFWwindow* application_window;
	SceneManager scene_manager;
	Renderer renderer;
	bool paused;

public:
	std::string application_name;

};

