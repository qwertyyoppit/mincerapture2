#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window
{
public:
	Window(const GLFWwindow* glwindow);

	void SetClearColor(glm::vec3 rgb, float a = 1.0f)
	{
		SetClearColor(rgb.x, rgb.y, rgb.z, a);
	}

	void SetClearColor(float r, float g, float b, float a = 1.0f);

	void SetSize(glm::ivec2 size)
	{
		SetSize(size.x, size.y);
	}

	void SetSize(int width, int height);

	const GLFWwindow& window;

	int MinWidth = 100;
	int MinHeight = 100;
};