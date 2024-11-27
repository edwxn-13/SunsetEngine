
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
	shadow_struct = setup_shadowmap();
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

	glBindTexture(GL_TEXTURE_2D, shadow_struct.depthMap);

	unsigned int shader = shader_manager.getSunsetShader(2)->getProgram();
	glm::mat4 view = glm::mat4(1.f);
	view = camera->getCamViewMatrix();

	Sun sun_obj = scene->scene_sun;

	glUniform3f(glGetUniformLocation(shader, "lightPos"), sun_obj.sun_pos.x, sun_obj.sun_pos.y, sun_obj.sun_pos.z);
	glUniform3f(glGetUniformLocation(shader, "lightDirection"), sun_obj.sun_dir.x, sun_obj.sun_dir.y, sun_obj.sun_dir.z);
	glUniform3f(glGetUniformLocation(shader, "lightColour"), sun_obj.sun_colour.x, sun_obj.sun_colour.y, sun_obj.sun_colour.z);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(sun_obj.lightMat));
	glUniformMatrix4fv(glGetUniformLocation(shader, "camMat"), 1, GL_FALSE, glm::value_ptr(view));

	shader_manager.getSunsetShader(2)->setFloat("depthBufferFC", camera->camDepthBufFC);
	


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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
//-----------------------------------------------------------------------------------------------------------------------------
	Sun s_sun = scene->scene_sun;
	float near_plane = 1.0f, far_plane = 12000;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	scene->scene_sun.sun_pos = (scene->scene_sun.o_pos - camera->getRootPosition()).glm();

	glm::mat4 lightView = glm::lookAt(s_sun.sun_pos.glm(), -camera->getRootPosition().glm(), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	scene->scene_sun.lightMat = lightSpaceMatrix;

	shader_manager.getSunsetShader(3)->useShader();

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_struct.FBO);

	glUniformMatrix4fv(glGetUniformLocation(shader_manager.getSunsetShader(3)->getProgram(), "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < scene->SceneMembers.size(); i++)
	{
		if (PlanetRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<PlanetRenderer>()) {
			if (mesh)
			{
				mesh->renderMesh(shader_manager.getSunsetShader(3)->getProgram());
				continue;
			}
		}

		if (MeshComponent* mesh = scene->SceneMembers[i]->getComponentOfType<MeshComponent>()) {
			if (mesh)
			{
				mesh->renderMesh(shader_manager.getSunsetShader(3)->getProgram());
				continue;

			}
		}

		if (CubeRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<CubeRenderer>()) {
			if (mesh)
			{
				mesh->renderMesh(shader_manager.getSunsetShader(3)->getProgram());
				continue;

			}
		}

		if (PlaneRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<PlaneRenderer>()) {
			if (mesh)
			{
				mesh->renderMesh(shader_manager.getSunsetShader(3)->getProgram());
				continue;

			}
		}

		if (MeshRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<MeshRenderer>()) {
			if (mesh)
			{
				mesh->renderMesh(shader_manager.getSunsetShader(3)->getProgram());
				continue;

			}
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Screen::getScreenX(), Screen::getScreenY());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//-----------------------------------------------------------------------------------------------------------------------------
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	//saveShadowMapToBitmap(shadow_struct.depthMap, SHADOW_WIDTH, SHADOW_HEIGHT);
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
	glBindTexture(GL_TEXTURE_2D, shadow_struct.depthMap);
	glUniform1i(glGetUniformLocation(shader, "Texture"), 0);

	glm::mat4 view = glm::mat4(1.f);
	view = camera->getCamViewMatrix();

	Sun sun_obj = scene->scene_sun;
	glUniform3f(glGetUniformLocation(shader, "lightPos"), sun_obj.sun_pos.x, sun_obj.sun_pos.y, sun_obj.sun_pos.z);
	glUniform3f(glGetUniformLocation(shader, "lightDirection"), sun_obj.sun_dir.x, sun_obj.sun_dir.y, sun_obj.sun_dir.z);
	glUniform3f(glGetUniformLocation(shader, "lightColour"), sun_obj.sun_colour.x, sun_obj.sun_colour.y, sun_obj.sun_colour.z);
	glUniformMatrix4fv(glGetUniformLocation(shader, "camMat"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(sun_obj.lightMat));


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
				continue;

			}
		}

		if (CubeRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<CubeRenderer>()) {
			if (mesh)
			{
				mesh->renderMesh(shader_manager.getSunsetShader(0)->getProgram());
				continue;

			}
		}

		if (PlaneRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<PlaneRenderer>()) {
			if (mesh)
			{
				mesh->renderMesh(shader_manager.getSunsetShader(0)->getProgram());
				continue;

			}
		}

		if (MeshRenderer* mesh = scene->SceneMembers[i]->getComponentOfType<MeshRenderer>()) {
			if (mesh)
			{
				mesh->renderMesh(shader_manager.getSunsetShader(0)->getProgram());
				continue;

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
	RenderShadows(scene);
	RenderSkybox(scene);
	CreateShadowMap(scene);
	RenderTrans(scene);
	RenderPlanets(scene);
	RenderGeneral(scene, deltaTime);
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
