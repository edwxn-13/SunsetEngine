#pragma once

class Scene;

struct SceneManager 
{

	void Start();
	void LoadScene();
	void NextScene();
	void PrevScene();

	Scene* active_scene;
};