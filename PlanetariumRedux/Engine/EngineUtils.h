#pragma once

#include "../Maths/MathHeader.h"
struct GLFWwindow;

class KeyCode 
{

};

class Screen 
{
	static int screen_x;
	static int screen_y;

public:
	static void setScreenXY(int ax, int ay);
	static int getScreenX();
	static int getScreenY();
};
class Input 
{
public:
	Input();
	int getScreenMouseX();
	int getScreenMouseY();
	Vector2f getMouseInputXY();
	void Update();
	static void updateWindowValue(GLFWwindow* swindow);

	void calcDelta();

	int OnKeyPressed(int KeyValue);
	int OnKeyDown(int KeyValue);
	int OnKeyUp(int KeyValue);
private:
	static GLFWwindow* window_p;
	GLFWwindow* window;

	Vector2f MouseXY;
	Vector2f deltaXY;

	Vector2f prev_pos = 0;

};


