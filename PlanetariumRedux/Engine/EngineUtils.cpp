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
	//if (glfwRawMouseMotionSupported())

	double a, b = 0;
	glfwGetCursorPos(window, &a, &b);

	int x_scale = Screen::getScreenX() / 2;
	int y_scale = Screen::getScreenY() / 2;

	double mouseRawX = a - (double)Screen::getScreenX()/2;
	double mouseRawY = b - (double)Screen::getScreenY() / 2;

	
	MouseXY.x = SunsetMath::Lerp(MouseXY.x, (mouseRawX/ (Screen::getScreenX() / 2)), 0.2f);
	MouseXY.y = SunsetMath::Lerp(MouseXY.y, (mouseRawY / (Screen::getScreenY() / 2)), 0.2f);
	
	glfwSetCursorPos(window, Screen::getScreenX() / 2, Screen::getScreenY() / 2);
}

void Input::updateWindowValue(GLFWwindow* swindow)
{
	window_p = swindow;
	glfwSetInputMode(swindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(swindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
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
