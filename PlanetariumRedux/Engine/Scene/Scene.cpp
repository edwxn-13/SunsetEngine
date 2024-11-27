#include "Scene.h"
#include "../../Utils/texture.h"
#include "../../SunsetCore.h"

std::vector<Scene*> Scene::SceneList = {};

Scene::Scene()
{
	SceneList.push_back(this);
	scene_sun.sun_pos = Vector3f(110.0f, 5200.0f, 15000.0f);
	scene_sun.sun_dir = Vector3f(1.0f, 1.0f, 1.0f);
	scene_sun.sun_colour = Vector3f(1.0f);
}

void Scene::InitScene()
{

}

void Scene::SetUpScene()
{
	for (int i = 0; i < SceneMembers.size(); i++)
	{
		SceneMembers[i]->Start();
		SceneMembers[i]->transform.Update(0);
	}
}

void Scene::UpdateScene(float deltaTime, int index)
{
	SceneMembers[index]->Update(deltaTime);
}

void Scene::FixedUpdate(float deltaTime, int index)
{
	SceneMembers[index]->FixedUpdate(deltaTime);
}

void Scene::StartScene()
{
	for (int i = 0; i < SceneMembers.size(); i++)
	{
		SceneMembers[i]->Start();
		SceneMembers[i]->transform.Update(0);
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

void Scene::Instantiate(EngineObject* engineObject, Vector3d position = 0, Vector3f rotation = 0)
{
	engineObject->transform.position = position;
	engineObject->transform.Rotate(rotation);
	attachToScene(engineObject);
	engineObject->Start();
}

Skybox* Scene::getSkybox()
{
	return &skybox;
}

