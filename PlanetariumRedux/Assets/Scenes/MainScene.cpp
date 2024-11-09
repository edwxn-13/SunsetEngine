#include "MainScene.h"
#include "../MeshObject/MeshObject.h"
#include "../Ship/ShipController/ShipController.h"
#include "../../Camera/camera.h"

MainScene::MainScene() : Scene()
{
	InitScene();
}

void MainScene::InitScene()
{
	SCamera Camera = SCamera();

	MeshObject gooch = MeshObject("objs/station/spaceStation.obj");
	MeshObject ship = MeshObject("objs/fighter/fighter.obj");

	ship.addComponent(new ShipController(&ship));
	gooch.transform.scale = Vector3f(22);
	gooch.localTransform.position = Vector3f(-200, 0, 0);
	ship.getTransform()->scale = Vector3f(5);
	ship.getTransform()->position = Vector3f(-50, -20.0f, 100.f);
	ship.addChild(&Camera);
}
