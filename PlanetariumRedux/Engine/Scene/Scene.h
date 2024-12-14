#pragma once
#include <vector>
#include <GL/gl3w.h>
#include "../../Utils/texture.h"
#include "../../Maths/MathHeader.h"
#include "../../Maths/Vector3d.h"
class EngineObject;
struct Sun
{
	Vector3d o_pos;
	Vector3f sun_pos;
	Vector3f sun_dir;
	Vector3f sun_colour;
	float star_class = 0.0000000009;
	glm::mat4 lightMat = 1.f;
};
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

	Sun scene_sun;

protected:
	Skybox skybox;
};