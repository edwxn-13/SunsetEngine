#pragma once
#include <vector>

class EngineObject;

class Scene
{

	virtual void InitScene();

	void SetUpScene();
	void UpdateScene();
	void FixedUpdate();
	void StartScene();

	void attachToScene(EngineObject* engineObject);

public:
	std::vector<EngineObject*> SceneMembers;

};