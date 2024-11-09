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
	static int getScreenMouseX();
	static int getScreenMouseY();
	static Vector2f getMouseInputXY();
	static void Update();
	static void updateWindowValue(GLFWwindow* window);

	static int OnKeyPressed(int KeyValue);
	static int OnKeyDown(int KeyValue);
	static int OnKeyUp(int KeyValue);
private:
	static GLFWwindow* window_p;
	static Vector2f MouseXY;
};


