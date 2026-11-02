#include "Window.h"
#include <iostream>

Window::Window(const GLFWwindow* glwindow) : window(*glwindow)
{
};

void Window::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Window::SetSize(int width, int height)
{

}