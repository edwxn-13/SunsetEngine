#pragma once
#include <vector>

class EngineObject;

class Scene
{
	static void LoadScene(Scene* scene);

	std::vector<EngineObject*> SceneMembers;

	void InitScene();
	void UpdateScene();
};