#include "Time.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

float Time::DeltaTime()
{
	return delta_time;
}

Time::Time()
{
	currentTime = 0.0f;
	delta_time = 0.0f;
	pastTime = 0.0f;
	tickrate = 80.0f;
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