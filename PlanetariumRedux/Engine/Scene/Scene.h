#pragma once
#include <vector>
#include <GL/gl3w.h>
#include "../../Utils/texture.h"

class EngineObject;

class Scene
{
	static std::vector<Scene*> SceneList;
public:

	Scene();

	virtual void InitScene();
	void SetUpScene();
	void UpdateScene(float deltaTime, int index);
	void FixedUpdate(float deltaTime, int index);
	void StartScene();

	void attachToScene(EngineObject* engineObject);

	static Scene* getScene(int index);

	std::vector<EngineObject*> SceneMembers;
	Skybox* getSkybox();
protected:

	Skybox skybox;
};