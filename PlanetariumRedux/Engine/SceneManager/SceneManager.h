#pragma once

class Scene;

struct SceneManager 
{
	SceneManager(int a);
	void Start();
	void LoadScene();
	void NextScene();
	void PrevScene();

	Scene* active_scene;
};