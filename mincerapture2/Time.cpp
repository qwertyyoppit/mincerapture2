#include <iostream>
#include "Time.h"
#include <GLFW/glfw3.h>

void Time::Update()
{
	double currenttime = glfwGetTime();
	_realDelta = currenttime - realTime;
	_realTime = currenttime;
	_delta = fmin(realDelta * timeScale, maximumDelta);
	_time += delta;
}