
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

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_struct.depthCubemap);

	unsigned int shader = shader_manager.getSunsetShader(2)->getProgram();
	glm::mat4 view = glm::mat4(1.f);
	view = camera->getCamViewMatrix();

	Sun sun_obj = scene->scene_sun;

	sun_obj.lightMat = glm::translate(glm::mat4(1.f), sun_obj.sun_pos.glm() - camera->transform.position.glm());

	glUniformMatrix4fv(glGetUniformLocation(shader, "camMat"), 1, GL_FALSE, glm::value_ptr(view));
	shader_manager.getSunsetShader(2)->setFloat("depthBufferFC", camera->camDepthBufFC);
	shader_manager.getSunsetShader(2)->setSun(sun_obj.sun_pos.glm() - camera->transform.position.glm(), sun_obj.star_class, sun_obj.y, sun_obj.quadratic);
	shader_manager.getSunsetShader(2)->setVector("camPos", Vector3f(0));
	shader_manager.getSunsetShader(2)->setFloat("far_plane", sh_far_plane);

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

				SunsetShader * atmosphere_shader = shader_manager.getSunsetShader(4);
				atmosphere_shader->useShader();
				atmosphere_shader->setFloat("depthBufferFC", camera->camDepthBufFC);
				atmosphere_shader->setMat("camMat", view);

				glm::mat4 temp_proj = glm::perspective(glm::radians(45.0f), (float)Screen::getScreenX() / (float)Screen::getScreenY(), 1.0f, 200000000.0f);
				temp_proj = glm::inverse(temp_proj);

				atmosphere_shader->setMat("inverseProjection", temp_proj);
				atmosphere_shader->setMat("inverseView", glm::translate(glm::mat4(1.f), camera->transform.position.glm()));

				atmosphere_shader->setVector("sphere_center", mesh->transform->position.glm() - camera->transform.position.glm());
				atmosphere_shader->setVector("planet.center", mesh->transform->position.glm() - camera->transform.position.glm());
				atmosphere_shader->setVector("cam_pos", camera->transform.position.glm());
				atmosphere_shader->setVector("cam_dir", camera->transform.forward());
				atmosphere_shader->setVector("sun_pos", sun_obj.sun_pos - camera->transform.position.glm());


				mesh->renderAtmospehre(atmosphere_shader);
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

	SunsetShader * shadow_shader = shader_manager.getSunsetShader(3);
//-----------------------------------------------------------------------------------------------------------------------------
	Sun s_sun = scene->scene_sun;
	glm::vec3 cam_pos = camera->getRootPosition().glm();
	glm::vec3 trans_sun_pos = s_sun.sun_pos.glm() - cam_pos;

	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, sh_near_plane, sh_far_plane);
	float shadowCamFC = 2.0 / (glm::log(sh_far_plane + 1.0) / 0.69314718055994530941723212145818f);

	std::vector<glm::mat4> shadowTransforms;

	shadowTransforms.push_back(shadowProj * glm::lookAt(trans_sun_pos, trans_sun_pos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(trans_sun_pos, trans_sun_pos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(trans_sun_pos, trans_sun_pos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(trans_sun_pos, trans_sun_pos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(trans_sun_pos, trans_sun_pos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(trans_sun_pos, trans_sun_pos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_struct.FBO);

	shadow_shader->useShader();
	shadow_shader->setMat("model", glm::translate(glm::mat4(1.f), trans_sun_pos));
	shadow_shader->setFloat("far_plane", sh_far_plane);
	shadow_shader->setFloat("shadowCamFC", shadowCamFC);

	shadow_shader->setVector("light_pos", trans_sun_pos);
	
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	for (unsigned int i = 0; i < 6; ++i) {

		std::string address = "shadowMatrices[" + std::to_string(i) + "]";
		shadow_shader->setMat(address.c_str(), shadowTransforms[i]);
	}

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

	//saveShadowMapToBitmap(GL_TEXTURE, SHADOW_WIDTH, SHADOW_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Screen::getScreenX(), Screen::getScreenY());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//-----------------------------------------------------------------------------------------------------------------------------
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

}

void Renderer::RenderTrans(Scene* scene)
{
}

void Renderer::RenderGeneral(Scene* scene, float deltaTime)
{
	glEnable(GL_DEPTH_TEST);

	if (deltaTime > 1) 
	{
		deltaTime = 0.001;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader_manager.getSunsetShader(0)->useShader();
	unsigned int shader = shader_manager.getSunsetShader(0)->getProgram();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_struct.depthCubemap);

	glUniform1i(glGetUniformLocation(shader, "depthMap"), shadow_struct.depthCubemap);

	glm::mat4 view = glm::mat4(1.f);
	view = camera->getCamViewMatrix();

	Sun sun_obj = scene->scene_sun;

	glUniformMatrix4fv(glGetUniformLocation(shader, "camMat"), 1, GL_FALSE, glm::value_ptr(view));

	shader_manager.getSunsetShader(0)->setFloat("depthBufferFC", camera->camDepthBufFC);
	shader_manager.getSunsetShader(0)->setSun(sun_obj.sun_pos.glm() - camera->transform.position.glm(), sun_obj.star_class, sun_obj.y, sun_obj.quadratic);
	shader_manager.getSunsetShader(0)->setVector("camPos",Vector3f(0));
	shader_manager.getSunsetShader(0)->setFloat("far_plane", sh_far_plane);

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
