#include "Scene.h"
#include "../../SunsetCore.h"

void Scene::InitScene()
{

}

void Scene::SetUpScene()
{

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
