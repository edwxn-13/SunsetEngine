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
	SCamera * Camera = new SCamera(this);

	MeshObject * gooch = new MeshObject("objs/station/spaceStation.obj", this);
	MeshObject * ship = new MeshObject("objs/fighter/fighter.obj", this);
	skybox = Skybox();
	skybox.setupCubemap();


	ship->addComponent(new ShipController(ship));
	gooch->transform.scale = Vector3f(22);
	gooch->localTransform.position = Vector3f(-200, 0, 0);
	ship->getTransform()->scale = Vector3f(5);
	ship->getTransform()->position = Vector3f(-50, -20.0f, 100.f);
	ship->addChild(Camera);

	attachToScene(Camera);
	attachToScene(gooch);
	attachToScene(ship);

}
