#pragma once
#include <vector>

class EngineObject;

class Skybox;

class Scene
{
	std::vector<EngineObject*> SceneMembers;
	Skybox* skybox;

	virtual void InitScene();

	void SetUpScene();
	void UpdateScene();
	void FixedUpdate();
	void StartScene();

	void attachToScene(EngineObject* engineObject);
};