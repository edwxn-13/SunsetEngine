#include "EngineUtils.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
GLFWwindow* Input::window_p = 0;
int Screen::screen_x = 0;
int Screen::screen_y = 0;

float Time::currentTime = 0.0f;
float Time::delta_time = 0.0f;
float Time::pastTime = 0.0f;
float Time::tickrate = 80.0f;

Vector2f Input::MouseXY = Vector2f(0);

void Screen::setScreenXY(int ax, int ay)
{
	screen_x = ax;
	screen_y = ay;
}


int Screen::getScreenX()
{
	return screen_x;
}

int Screen::getScreenY()
{
	return screen_y;
}


int Input::getScreenMouseX()
{
	double a, b = 0;
	glfwGetCursorPos(window_p, &a, &b);
	return a;
}

int Input::getScreenMouseY()
{
	double a, b = 0;
	glfwGetCursorPos(window_p, &a , &b);
	return b;
}

Vector2f Input::getMouseInputXY()
{
	return MouseXY;
}


void Input::Update()  
{
	//if (glfwRawMouseMotionSupported()) 
	glfwSetInputMode(window_p, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	double a, b = 0;
	glfwGetCursorPos(window_p, &a, &b);

	int x_scale = Screen::getScreenX() / 2;
	int y_scale = Screen::getScreenY() / 2;

	double mouseRawX = a - Screen::getScreenX()/2;
	double mouseRawY = b - Screen::getScreenY() / 2;

	MouseXY.x = mouseRawX/ (Screen::getScreenX() / 2);
	MouseXY.y = (mouseRawY / (Screen::getScreenY() / 2));
	glfwSetCursorPos(window_p, Screen::getScreenX() / 2, Screen::getScreenY() / 2);
}

void Input::updateWindowValue(GLFWwindow* window)
{
	window_p = window;
}

int Input::OnKeyPressed(int KeyValue)
{
	return (glfwGetKey(window_p, KeyValue) == GLFW_PRESS);
}

int Input::OnKeyDown(int KeyValue)
{
	return glfwGetKey(window_p, KeyValue) == GLFW_RELEASE;
}

int Input::OnKeyUp(int KeyValue)
{
	return glfwGetKey(window_p, KeyValue) == GLFW_RELEASE;
}

float Time::DeltaTime()
{
	return delta_time;
}

bool Time::fixedUpdateChecker()
{
	if (currentTime > 1 / tickrate) 
	{
		printf("this is the time step: %f \n\n", currentTime);
		currentTime = 0;
		return true;
	}
	return false;
}

void Time::updateTime()
{
	float time = (float)glfwGetTime();
	delta_time = time - pastTime;
	pastTime = time;
	currentTime += delta_time;
}
