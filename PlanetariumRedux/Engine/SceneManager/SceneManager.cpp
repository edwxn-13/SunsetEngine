#include "SceneManager.h"
#include "../../Assets/Scenes/MainScene.h"

SceneManager::SceneManager(int a)
{
	Start();
}

void SceneManager::Start()
{
	MainScene * game_scene = new MainScene();
	active_scene = game_scene;
	active_scene->StartScene();
	
}

void SceneManager::LoadScene()
{

}

void SceneManager::NextScene()
{

}

void SceneManager::PrevScene()
{

}
