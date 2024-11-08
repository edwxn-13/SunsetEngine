#pragma once

class GLFWwindow;
class Scene;

class Renderer 
{

	Renderer(GLFWwindow* window);


	void CreateShadowMap(Scene* scene);
	void RenderShadows(Scene* scene);
	void TransRender(Scene* scene);
	void GeneralRender(Scene* scene);

	void setUpShaders();
	void preRenderSetUp(Scene * scene);
	void clear();
	
};