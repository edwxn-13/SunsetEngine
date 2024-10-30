#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Utils/error.h"
#include "Utils/shader.h"
#include "Utils/shadow.h"
#include "Camera/camera.h"
#include "Utils/texture.h"
#include "Components/MeshComponent/MeshComponent.h"
#include "Assets/MeshObject/MeshObject.h"
#include "Components/Rigidbody/Rigidbody.h"
#include "Engine/EngineUtils.h"

//light direction variable here

SCamera Camera = SCamera();
glm::vec3 lightDirection = glm::vec3(0.1f, -0.81f, -0.61f);
glm::vec3 lightPos = glm::vec3(2.f, 6.f, 7.f);

#define NUM_BUFFERS 1
#define NUM_VAOS 1
GLuint Buffers[NUM_BUFFERS];
GLuint VAOs[NUM_VAOS];

#define WIDTH 1024
#define HEIGHT 768

#define SH_MAP_WIDTH 2048
#define SH_MAP_HEIGHT 2048

float yaw, pitch = 0;
float roll = 0.0f;

void drawFloorAndCubes(unsigned int shaderProgram)
{
	glBindVertexArray(VAOs[0]);

	//floor
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(0, -3, 0));
	model = glm::scale(model, glm::vec3(100, 0.1, 100));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cubes
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			for (int k = -1; k < 2; k++)
			{
				glm::mat4 model = glm::mat4(1.f);
				model = glm::translate(model, glm::vec3(float(i * 2), float(j * 2), float(k * 2)));
				glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
	}

}

void preRenderSetUp() 
{
	for (int i = 0; i < EngineObject::getObjectListSize(); i++)
	{
		EngineObject::getGlobalObjectIndex(i)->transform.Update();
		if (MeshComponent* mesh = EngineObject::getGlobalObjectIndex(i)->getComponentOfType<MeshComponent>()) {
			if (mesh)
			{
				mesh->loadMesh();
				mesh->setUpMesh();
			}
		}
	}
}

void renderWithShadow(unsigned int renderShaderProgram, ShadowStruct shadow, glm::mat4 projectedLightSpaceMatrix)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(renderShaderProgram);
	glUniform3f(glGetUniformLocation(renderShaderProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(glGetUniformLocation(renderShaderProgram, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
	glUniform3f(glGetUniformLocation(renderShaderProgram, "lightColour"), 0.f, 0.f, 0.f);
	glUniform3f(glGetUniformLocation(renderShaderProgram, "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);
	glm::mat4 view = glm::mat4(1.f);
	view = Camera.getCamViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(renderShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, .01f, 100.f);
	glUniformMatrix4fv(glGetUniformLocation(renderShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	drawFloorAndCubes(renderShaderProgram);
}

void renderWithTexture(unsigned int shader, ShadowStruct shadow, glm::mat4 projectedLightSpaceMatrix)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(shader);
	glUniform1i(glGetUniformLocation(shader, "Texture"), 0);

	glm::mat4 view = glm::mat4(1.f);
	view = Camera.getCamViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, .01f, 10000.f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniform3f(glGetUniformLocation(shader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glUniform3f(glGetUniformLocation(shader, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
	glUniform3f(glGetUniformLocation(shader, "lightColour"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader, "camPos"), Camera.transform.position.x, Camera.transform.position.y, Camera.transform.position.z);

	for (int i = 0; i < Component::getComponentListSize(); i++)
	{
		//MeshComponent* temp;
		//if(temp == (MeshComponent*)Component::getGlobalComponantIndex(i))
		if (Component::getGlobalComponantIndex(i)->isActive()) 
		{
			Component::getGlobalComponantIndex(i)->Update();
			if (((MeshComponent*)Component::getGlobalComponantIndex(i))->getType() != shader) { continue; }
			((MeshComponent*)Component::getGlobalComponantIndex(i))->renderMesh();
		}
		
	}
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(0);
}

/*
void renderWithTextureP(unsigned int shader, vector<Planet>eObjs, ShadowStruct shadow, glm::mat4 projectedLightSpaceMatrix)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(shader);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, .01f, 10000.f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	glUniform3f(glGetUniformLocation(shader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glUniform3f(glGetUniformLocation(shader, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
	glUniform3f(glGetUniformLocation(shader, "lightColour"), 0.5f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shader, "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);

	for (int i = 0; i < eObjs.size(); i++)
	{
		//if (eObjs[i].getType() != shader) { continue; }
		eObjs[i].renderObj();
	}
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

}*/
/*
void renderOceanP(unsigned int shader, vector<Planet>eObjs)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(shader);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, .01f, 10000.f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	glUniform3f(glGetUniformLocation(shader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glUniform3f(glGetUniformLocation(shader, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
	glUniform3f(glGetUniformLocation(shader, "lightColour"), 0.5f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shader, "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);

	for (int i = 0; i < eObjs.size(); i++)
	{
		//if (eObjs[i].getType() != shader) { continue; }
		eObjs[i].renderOcean();
	}
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

}*/

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE && action) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float x_offset = 0.f;
	float y_offset = 0.f;
	bool cam_changed = false;

	if (glfwGetKey(window, GLFW_KEY_RIGHT && action) == GLFW_PRESS)
	{
		x_offset = 1.f;
		y_offset = 0.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT && action) == GLFW_RELEASE)
	{
		x_offset = -1.f;
		y_offset = 0.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_UP && action) == GLFW_RELEASE)
	{
		x_offset = 0.f;
		y_offset = 1.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN && action) == GLFW_RELEASE)
	{
		x_offset = 0.f;
		y_offset = -1.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_R && action) == GLFW_REPEAT)
	{
		Camera.cam_dist += 0.1f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_F && action) == GLFW_REPEAT)
	{
		Camera.cam_dist -= 0.1f;
		cam_changed = true;
	}

	if (cam_changed)
	{
	}
}

void SizeCallback(GLFWwindow* window, int w, int h)
{
	Screen::setScreenXY(WIDTH, HEIGHT);
	glViewport(0, 0, w, h);
}

void processKeyboard(GLFWwindow* window, Vector3f target)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		lightPos = Camera.transform.position.glm();
		lightDirection = Camera.transform.forward().glm();
	}
}

void generateDepthMap(unsigned int shadowShaderProgram, ShadowStruct shadow, glm::mat4 projectedLightSpaceMatrix)
{
	glViewport(0, 0, SH_MAP_WIDTH, SH_MAP_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow.FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(projectedLightSpaceMatrix));
	//drawFloorAndCubes(shadowShaderProgram);
	renderWithTexture(shadowShaderProgram, shadow, projectedLightSpaceMatrix);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main(int argc, char** argv)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Planetarium!", NULL, NULL);
	glfwMakeContextCurrent(window);
	Screen::setScreenXY(WIDTH, HEIGHT);
	Input::updateWindowValue(window);

	//glfwSetKeyCallback(window, KeyCallback);

	glfwSetWindowSizeCallback(window, SizeCallback);

	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);

	ShadowStruct shadow = setup_shadowmap(SH_MAP_WIDTH, SH_MAP_HEIGHT);

	GLuint program = CompileShader("Shaders/PhongLight/phong.vert", "Shaders/PhongLight/phong.frag");
	GLuint shadow_program = CompileShader("Shaders/Shadow/shadow.vert", "Shaders/Shadow/shadow.frag");

	GLuint texture_program = CompileShader("Shaders/Textured/textured.vert", "Shaders/Textured/textured.frag");
	GLuint skybox_shader = CompileShader("Shaders/Skybox/skybox.vert", "Shaders/Skybox/skybox.frag");
	GLuint simple_planet = CompileShader("Shaders/SimplePlanet/simplePlanetShader.vert", "Shaders/SimplePlanet/simplePlanetShader.frag");


	glDebugMessageCallback(DebguMessageCallback, 0);

	Camera.InitCamera();
	Camera.cam_dist = 5.f;
	
	//SECTION A - EDIT THIS CODE TO TEST

	MeshObject gooch = MeshObject("objs/station/spaceStation.obj", texture_program);
	MeshObject ship = MeshObject("objs/MagnumTris/MH2A.obj", texture_program);
	gooch.transform.scale = Vector3f(0.2);
	gooch.localTransform.position = Vector3f(100, 50, 2);
	ship.getTransform()->scale = Vector3f(5);
	ship.getTransform()->position = Vector3f(0, -1.8f, -1.f);
	ship.transform.Rotate(Vector3f(0));

	
	ship.addChild(&Camera);
	//ship.addChild(&Camera);

	preRenderSetUp();

	Skybox skybox = Skybox();
	skybox.setupCubemap();

	glEnable(GL_DEPTH_TEST);
	//Camera.LookAt(glm::vec3(0, 0, 0), Camera.cam_dist, 0.f, 0.f);

	while (!glfwWindowShouldClose(window))
	{
		float near_plane = 1.0f, far_plane = 70.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projectedLightSpaceMatrix = lightProjection * lightView;

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, WIDTH, HEIGHT);
		Input::Update();
		skybox.renderCubemap(skybox_shader, &Camera);
		renderWithTexture(texture_program, shadow, projectedLightSpaceMatrix);
		glfwSwapBuffers(window);
		glfwPollEvents();
		processKeyboard(window, ship.transform.position);

	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}