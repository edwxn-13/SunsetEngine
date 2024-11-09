#include "Scene.h"
#include "../../SunsetCore.h"

std::vector<Scene*> Scene::SceneList = {};

Scene::Scene()
{
	SceneList.push_back(this);
}

void Scene::InitScene()
{

}

void Scene::SetUpScene()
{
	for (int i = 0; i < SceneMembers.size(); i++)
	{
		SceneMembers[i]->Start();
		SceneMembers[i]->transform.Update();
	}
}

void Scene::UpdateScene()
{

}

void Scene::FixedUpdate()
{
	for (int i = 0; i < SceneMembers.size(); i++)
	{
		SceneMembers[i]->Start();
		SceneMembers[i]->transform.Update();
	}
}

void Scene::StartScene()
{
	for (int i = 0; i < SceneMembers.size(); i++)
	{
		SceneMembers[i]->Start();
		SceneMembers[i]->transform.Update();
	}
}

void Scene::attachToScene(EngineObject* engineObject)
{
	SceneMembers.push_back(engineObject);
}

Scene* Scene::getScene(int index)
{
	return SceneList[index];
}

Skybox* Scene::getSkybox()
{
	return &skybox;
}

