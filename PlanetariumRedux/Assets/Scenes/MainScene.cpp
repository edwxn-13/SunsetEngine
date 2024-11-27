#include "MainScene.h"
#include "../MeshObject/MeshObject.h"
#include "../MeshObject/SMeshObject.h"

#include "../Ship/ShipController/ShipController.h"
#include "../../Camera/camera.h"

#include "../../SunsetCore.h"
#include "../../EngineObjects/Planet/Planet.h"

MainScene::MainScene() : Scene()
{
	InitScene();
	scene_sun.sun_pos = Vector3f(0, 0, -15000.0f);
	scene_sun.o_pos = Vector3d(0, 0, -15000.0f);

	scene_sun.sun_dir = Vector3f(1.0f, 1.0f, 1.0f);
	scene_sun.sun_colour = Vector3f(1.0f);
}

void MainScene::InitScene()
{
	SCamera * Camera = new SCamera(this);


	SMeshObject * helemt = new SMeshObject("Assets/Models/ProtoHelmet/proto_helmet.dae", this);
	helemt->transform.Rotate(Vector3f(0,180,0));
	helemt->transform.scale = 40;
	helemt->transform.Translate(Vector3d(50,0,0));

	SMeshObject* ship = new SMeshObject("objs/fighter/fighter.obj", this);

	skybox = Skybox();
	skybox.setupCubemap();

	Cube* cube = new Cube(this, 2000, 10, 2000);
	cube->transform.scale = Vector3f(9);
	//attachToScene(cube);

	ship->addComponent(new ShipController(ship));
	ship->getTransform()->scale = Vector3f(1);
	ship->getTransform()->position = Vector3d(0, 0, 100);
	ship->addChild(Camera);

	Planet* planet = new Planet(this);

	planet->transform.Translate(Vector3d(645000,0,0));

	planet->transform.scale = 100000;

	attachToScene(planet);
	attachToScene(Camera);
	attachToScene(helemt);
	attachToScene(ship);


}
