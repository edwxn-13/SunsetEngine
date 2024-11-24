#pragma once
#include <vector>
#include <GL/gl3w.h>
#include "../../Utils/texture.h"
#include "../../Maths/MathHeader.h"
#include "../../Maths/Vector3d.h"
class EngineObject;

class Scene
{
	static std::vector<Scene*> SceneList;
public:

	Scene();

	virtual void InitScene();
	void SetUpScene();
	void UpdateScene(float deltaTime, int index);
	void FixedUpdate(float deltaTime, int index);
	void StartScene();

	void attachToScene(EngineObject* engineObject);

	static Scene* getScene(int index);

	void Instantiate(EngineObject * engineObject, Vector3d position, Vector3f rotation);

	std::vector<EngineObject*> SceneMembers;
	Skybox* getSkybox();
protected:

	Vector3f sun_pos;
	Skybox skybox;
};