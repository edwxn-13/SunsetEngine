#include "SceneManager.h"
#include "../../Assets/Scenes/MainScene.h"

void SceneManager::Start()
{
	MainScene game_scene = MainScene();
	active_scene = &game_scene;
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
