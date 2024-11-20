#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Engine/EngineUtils.h"
#include "Engine/Engine.h"
#include "Utils/error.h"
#define WIDTH 1024
#define HEIGHT 900

#define SH_MAP_WIDTH 2048
#define SH_MAP_HEIGHT 2048

void SizeCallback(GLFWwindow* window, int w, int h)
{
	Screen::setScreenXY(w, h);
	glViewport(0, 0, w, h);
}

void EngineLaunch() 
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Sunset Engine v1.10", NULL, NULL);
	glfwMakeContextCurrent(window);
	Screen::setScreenXY(WIDTH, HEIGHT);
	Input::updateWindowValue(window);
	glfwSetWindowSizeCallback(window, SizeCallback);
	gl3wInit();
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebguMessageCallback, 0);
	Engine engine = Engine(window);
	engine.EngineStart();
}

int main(int argc, char** argv){
	EngineLaunch();
	return 0;
}