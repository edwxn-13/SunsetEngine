#include "MainScene.h"
#include "../MeshObject/MeshObject.h"
#include "../MeshObject/SMeshObject.h"

#include "../Ship/ShipController/ShipController.h"
#include "../../Camera/camera.h"

#include "../../SunsetCore.h"

MainScene::MainScene() : Scene()
{
	InitScene();
}

void MainScene::InitScene()
{
	SCamera * Camera = new SCamera(this);

	//MeshObject * gooch = new MeshObject("objs/station/spaceStation.obj", this);

	SMeshObject * helemt = new SMeshObject("Assets/Models/ProtoHelmet/proto_helmet.dae", this);
	helemt->transform.Rotate(Vector3f(0,180,0));
	//SMeshObject * s_ship = new SMeshObject("objs/fighter/fighter.obj", this);
	helemt->transform.scale = 40;
	helemt->transform.Translate(Vector3f(50,0,0));
	MeshObject* ship = new MeshObject("objs/fighter/fighter.obj", this);

	skybox = Skybox();
	skybox.setupCubemap();


	Plane* plane = new Plane(this);
	plane->transform.scale = Vector3f(20);
	attachToScene(plane);

	ship->addComponent(new ShipController(ship));
	//gooch->transform.scale = Vector3f(22);
	//gooch->localTransform.position = Vector3f(-200, 0, 0);
	ship->getTransform()->scale = Vector3f(5);
	ship->getTransform()->position = Vector3f(0, 0, 0);
	ship->addChild(Camera);

	attachToScene(Camera);
	//attachToScene(gooch);
	attachToScene(helemt);
	attachToScene(ship);


}
