#pragma once
#include "ShaderManager/ShaderManager.h"

struct GLFWwindow;
class Scene;
class SCamera;
class Renderer 
{
public:

	Renderer();
	Renderer(GLFWwindow* window);

	void setActiveCamera();

	void RenderSkybox(Scene* scene);
	void CreateShadowMap(Scene* scene);
	void RenderShadows(Scene* scene);
	void RenderTrans(Scene* scene);
	void RenderGeneral(Scene* scene, float deltaTime);

	void RenderLoop(Scene * scene, float deltaTime);

	void setUpShaders();
	void preRenderSetUp(Scene * scene);
	void clear();

	ShaderManager shader_manager;
	GLFWwindow* app_window;

private:

	float RenderingDistance;
	SCamera* camera;
};
