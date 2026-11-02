#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "Game.h"
#include "Time.h"
#include "Constants.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Game Blockheads = Game(800, 600);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Blockheads", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeLimits(window, MIN_WIDTH, MIN_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    glClearColor(0.25f, 0.25f, 0.35f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Blockheads.Init();

    glm::vec2 cursorpos(0);
    glm::vec2 cursordelta(0);

    while (!glfwWindowShouldClose(window))
    {
        Time::Update();

        double curx, cury;
        glfwGetCursorPos(window, &curx, &cury);
        glm::vec2 curcurpos(curx, cury);
        cursordelta = curcurpos - cursorpos;
        cursorpos = curcurpos;
        Blockheads.CursorPosition = cursorpos;
        Blockheads.CursorDelta = cursordelta;

        Blockheads.ProcessInput();

        Blockheads.Update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Blockheads.Render();
        
        if (Time::delta > 1.0f) glfwSetWindowShouldClose(window, true);

        for (auto& key : Blockheads.KeyDown) key = false;
        for (auto& key : Blockheads.KeyReleased) key = false;
        for (auto& button : Blockheads.MouseDown) button = false;
        for (auto& button : Blockheads.MouseReleased) button = false;
        Blockheads.ScrollY = 0;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    width = std::max(width, MIN_WIDTH);
    height = std::max(height, MIN_HEIGHT);
    glViewport(0, 0, width, height);
    Blockheads.Width = width;
    Blockheads.Height = height;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            Blockheads.KeyDown[key] = true;
            Blockheads.KeyHeld[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            Blockheads.KeyHeld[key] = false;
            Blockheads.KeyReleased[key] = true;
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button >= 0 && button < 8)
    {
        if (action == GLFW_PRESS)
        {
            Blockheads.MouseDown[button] = true;
            Blockheads.MouseHeld[button] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            Blockheads.MouseHeld[button] = false;
            Blockheads.MouseReleased[button] = true;
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Blockheads.ScrollY = yoffset;
}
