#include "MainScene.h"
#include "../MeshObject/MeshObject.h"
#include "../MeshObject/SMeshObject.h"

#include "../Ship/ShipController/ShipController.h"
#include "../../Camera/camera.h"

#include "../../SunsetCore.h"
#include "../../EngineObjects/Planet/Planet.h"
#include "../../Components/PlanetRenderer/PlanetRenderer.h"

MainScene::MainScene() : Scene()
{
	InitScene();
	scene_sun.sun_pos = Vector3f(-10, 0, 0);
	scene_sun.o_pos = Vector3d(-10, 0, 0);
	scene_sun.star_class = 0.00007f;
	scene_sun.y = 1.0f;
	scene_sun.quadratic = 0.000017f;
	scene_sun.sun_dir = Vector3f(1.0f, 1.0f, 1.0f);
	scene_sun.sun_colour = Vector3f(1.0f);
}

void MainScene::InitScene()
{
	SCamera * Camera = new SCamera(this);


	SMeshObject * helemt = new SMeshObject("Assets/Models/ProtoHelmet/proto_helmet.dae", this);
	helemt->transform.Rotate(Vector3f(0,180,0));
	helemt->transform.scale = 40;
	helemt->transform.Translate(Vector3d(50,152,0));

	skybox = Skybox();
	skybox.setupCubemap();

	Cube* cube = new Cube(this, 2000, 10, 2000);
	cube->transform.scale = Vector3f(9,4,9);
	cube->transform.Translate(Vector3f(1,-60,9));

	attachToScene(cube);

	/*
	PlanetSettings settings;
	Planet* planet = new Planet(this);
	planet->transform.Translate(Vector3d(2100000,700000,6));
	attachToScene(planet);*/

	attachToScene(Camera);
	attachToScene(helemt);
	//attachToScene(ship);


}
