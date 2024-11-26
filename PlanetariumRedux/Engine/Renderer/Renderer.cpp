
#include "Renderer.h"
#include "../../Utils/shader.h"
#include "../Renderer/ShaderManager/ShaderManager.h"
#include "../Scene/Scene.h"
#include "../../Components/RenderingComponent.h"
#include "../../Components/MeshComponent/MeshComponent.h"
#include "../../Components/PrimativeRenderer/PrimativeRenderer.h"

#include "../../Components/MeshRenderer/MeshRenderer.h"
#include "../../EngineObjects/EngineObject.h"
#include "../../Camera/camera.h"
#include <iostream>
#include <GL/gl3w.h>
#include <glm/gtc/type_ptr.hpp>
#include "../EngineUtils.h"
#include "../../Utils/texture.h"
#include "../../Components/PlanetRenderer/PlanetRenderer.h"


Renderer::Renderer()
{

}

Renderer::Renderer(GLFWwindow * window)
{
	app_window = window;
	RenderingDistance = 2000;
	camera = SCamera::getSceneCamera();
	shader_manager = ShaderManager();
	shader_manager.setupShaders();
}

void Renderer::setActiveCamera()
{
	camera = SCamera::getSceneCamera();
}

void Renderer::RenderSkybox(Scene* scene)
{
	scene->getSkybox()->renderCubemap(shader_manager.getSunsetShader(1)->getProgram(), camera);
}

void Renderer::RenderPlanets(Scene* scene)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader_manager.getSunsetShader(2)->useShader();
	unsigned int shader = shader_manager.getSunsetShader(0)->getProgram();
	glm::mat4 view = glm::mat4(1.f);
	view = camera->getCamViewMatrix();

	glUniform3f(glGetUniformLocation(shader, "lightPos"), 110.0f, 5200.0f, 15000.0f);
	glUniform3f(glGetUniformLocation(shader, "lightDirection"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader, "lightColour"), 1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "camMat"), 1, GL_FALSE, glm::value_ptr(view));

	for (int i = 0; i < scene->SceneMembers.size(); i++)
	{
		float distance_from_cam = (scene->SceneMembers[i]->transform.position - camera->transform.position).magnitude();
		if (distance_from_cam > RenderingDistance)
		{
		}

		if (PlanetRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<PlanetRenderer>()) {
			if (mesh)
			{
				mesh->renderMesh(shader_manager.getSunsetShader(2)->getProgram());
			}
		}
	}
}

void Renderer::CreateShadowMap(Scene* scene)
{
}

void Renderer::RenderShadows(Scene* scene)
{
}

void Renderer::RenderTrans(Scene* scene)
{
}

void Renderer::RenderGeneral(Scene* scene, float deltaTime)
{

	if (deltaTime > 1) 
	{
		deltaTime = 0.001;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader_manager.getSunsetShader(0)->useShader();
	unsigned int shader = shader_manager.getSunsetShader(0)->getProgram();
	glUniform1i(glGetUniformLocation(shader, "Texture"), 0);

	glm::mat4 view = glm::mat4(1.f);
	view = camera->getCamViewMatrix();

	glUniform3f(glGetUniformLocation(shader, "lightPos"), 110.0f, 5200.0f, 15000.0f);
	glUniform3f(glGetUniformLocation(shader, "lightDirection"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader, "lightColour"), 1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "camMat"), 1, GL_FALSE, glm::value_ptr(view));

	for (int i = 0; i < scene->SceneMembers.size(); i++)
	{
		scene->UpdateScene(deltaTime, i);
		scene->FixedUpdate(deltaTime, i);

		float distance_from_cam = (scene->SceneMembers[i]->transform.position - camera->transform.position).magnitude();
		if (distance_from_cam > RenderingDistance)
		{
		}

		if (MeshComponent* mesh = scene->SceneMembers[i]->getComponentOfType<MeshComponent>()) {
			if (mesh)
			{
				mesh->renderMesh(shader_manager.getSunsetShader(0)->getProgram());
			}
		}

		if (CubeRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<CubeRenderer>()) {
			if (mesh)
			{
				mesh->renderMesh(shader_manager.getSunsetShader(0)->getProgram());
			}
		}

		if (PlaneRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<PlaneRenderer>()) {
			if (mesh)
			{
				mesh->renderMesh(shader_manager.getSunsetShader(0)->getProgram());
			}
		}

		if (MeshRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<MeshRenderer>()) {
			if (mesh)
			{
				mesh->renderMesh(shader_manager.getSunsetShader(0)->getProgram());
			}
		}
	}
}

void Renderer::RenderDebug(Scene* scene, float deltaTime)
{

}

void Renderer::RenderLoop(Scene* scene, float deltaTime)
{
	clear();
	RenderSkybox(scene);
	CreateShadowMap(scene);
	RenderTrans(scene);
	RenderPlanets(scene);
	RenderGeneral(scene, deltaTime);
	RenderShadows(scene);
}

void Renderer::setUpShaders()
{
	SunsetShader("Shaders/Textured/textured.vert", "Shaders/Textured/textured.frag");
	SunsetShader("Shaders/Skybox/skybox.vert", "Shaders/Skybox/skybox.frag");
	SunsetShader("Shaders/PhongLight/phong.vert", "Shaders/PhongLight/phong.frag");
	SunsetShader("Shaders/Shadow/shadow.vert", "Shaders/Shadow/shadow.frag");
	SunsetShader("Shaders/SimplePlanet/simplePlanetShader.vert", "Shaders/SimplePlanet/simplePlanetShader.frag");
}

void Renderer::preRenderSetUp(Scene* scene)
{
	SCamera::getSceneCamera();
	for (int i = 0; i < scene->SceneMembers.size(); i++)
	{
		if (MeshComponent* mesh = scene->SceneMembers[i]->getComponentOfType<MeshComponent>()) {
			if (mesh)
			{
				mesh->loadMesh();
				mesh->setUpMesh();
			}
		}

		if (PlaneRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<PlaneRenderer>()) {
			if (mesh)
			{
				mesh->loadMesh();
				mesh->setUpMesh();
			}
		}

		if (PlanetRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<PlanetRenderer>()) {
			if (mesh)
			{
				mesh->loadMesh();
				mesh->setUpMesh();
			}
		}

		if (CubeRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<CubeRenderer>()) {
			if (mesh)
			{
				mesh->loadMesh();
				mesh->setUpMesh();
			}
		}
	}
}

void Renderer::clear()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, Screen::getScreenX(), Screen::getScreenY());
}
