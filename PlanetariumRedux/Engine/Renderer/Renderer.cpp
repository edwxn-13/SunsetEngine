
#include "Renderer.h"
#include "../../Utils/shader.h"
#include "../Renderer/ShaderManager/ShaderManager.h"
#include "../Scene/Scene.h"
#include "../../Components/RenderingComponent.h"
#include "../../Components/MeshComponent/MeshComponent.h"

#include "../../EngineObjects/EngineObject.h"
#include "../../Camera/camera.h"
#include <iostream>
#include <GL/gl3w.h>
#include <glm/gtc/type_ptr.hpp>
#include "../EngineUtils.h"


Renderer::Renderer()
{
	shader_manager = ShaderManager();
	shader_manager = ShaderManager();
	shader_manager.setupShaders();
}

Renderer::Renderer(GLFWwindow * window)
{
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

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader_manager.getSunsetShader(0)->useShader();
	unsigned int shader = shader_manager.getSunsetShader(0)->getProgram();
	glUniform1i(glGetUniformLocation(shader, "Texture"), 0);

	glm::mat4 view = glm::mat4(1.f);
	view = camera->getCamViewMatrix();

	glUniform3f(glGetUniformLocation(shader, "lightPos"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader, "lightDirection"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader, "lightColour"), 1.0f, 1.0f, 1.0f);

	glUniformMatrix4fv(glGetUniformLocation(shader, "camMat"), 1, GL_FALSE, glm::value_ptr(view));
	glUniform3f(glGetUniformLocation(shader, "camPos"), camera->transform.position.x, camera->transform.position.y, camera->transform.position.z);

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
	}
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);
}

void Renderer::RenderLoop(Scene* scene, float deltaTime)
{
	clear();
	RenderSkybox(scene);
	CreateShadowMap(scene);
	RenderTrans(scene);
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
	}
}

void Renderer::clear()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, Screen::getScreenX(), Screen::getScreenY());
}
