#include "EngineUtils.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
GLFWwindow* Input::window_p = 0;
int Screen::screen_x = 0;
int Screen::screen_y = 0;

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

Input::Input()
{
	window = window_p;
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
	double a, b = 0;
	glfwGetCursorPos(window, &a, &b);

	int x_scale = Screen::getScreenX() / 2;
	int y_scale = Screen::getScreenY() / 2;

	double mouseRawX = a - (double)Screen::getScreenX()/2;
	double mouseRawY = b - (double)Screen::getScreenY() / 2;
	
	MouseXY.x = mouseRawX / ((double)Screen::getScreenX() / 2);
	MouseXY.y = mouseRawY / ((double)Screen::getScreenY() / 2);


	//std::cout << mouseRawX << " , " << mouseRawY << std::endl;
	glfwSetCursorPos(window, Screen::getScreenX()/2, Screen::getScreenY() / 2);

	calcDelta();
}

void Input::updateWindowValue(GLFWwindow* swindow)
{
	window_p = swindow;
	glfwSetInputMode(swindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetInputMode(swindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Input::calcDelta()
{
	deltaXY = MouseXY - prev_pos;
	prev_pos = MouseXY;
}

int Input::OnKeyPressed(int KeyValue)
{
	return (glfwGetKey(window, KeyValue) == GLFW_PRESS);
}

int Input::OnKeyDown(int KeyValue)
{
	return glfwGetKey(window, KeyValue) == GLFW_RELEASE;
}

int Input::OnKeyUp(int KeyValue)
{
	return glfwGetKey(window, KeyValue) == GLFW_RELEASE;
}
