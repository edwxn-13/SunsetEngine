#pragma once

class GLFWwindow;
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
	void RenderGeneral(Scene* scene);

	void RenderLoop(Scene * scene);

	void setUpShaders();
	void preRenderSetUp(Scene * scene);
	void clear();

private:

	float RenderingDistance;
	SCamera* camera;
	GLFWwindow* app_window;
};
