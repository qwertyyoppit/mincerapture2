#include "Game.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Time.h"
#include "includes/shader.h"
#include "Camera.h"
#include "CameraController.h"
#include "Texture.h"
#include "LoadTextureFromFile.h"
#include "InitBlockData.h"
#include "Entity.h"
#include "DroppedItem.h"
#include <shared_mutex>
#include "Collision.h"

Shader* shader;
Texture2D TileMap;
Camera camera = Camera();
CameraController cameraControl = CameraController(camera);

//TODO: store settings seperately
float CameraSensitivity = 100.f;
bool Freecam;

std::vector<std::unique_ptr<Entity>> Entities;

Game::Game(unsigned int width, unsigned int height) : Width(width), Height(height)
{
	
}

Game::~Game()
{

}

void Game::Init()
{
    //TODO: next time, load from hypothetical asset menu
    TileMap = LoadTextureFromFile("images/TileMap.png", true);
    shader = new Shader("shaders/shader.vert", "shaders/shader.frag");
    camera.Position.z = 10.f;
    camera.Position.y = 30.f;
    cameraControl.Distance = 10;
    cameraControl.Decelerate = 5.0f;
    camera.FieldOfView = 45.0f;
    
    InitBlockData::Init();
    glClearColor(0.549f, 0.831f, 1.f, 1.0f);
    glClearColor(0.063, 0.078, 0.122, 1.0f);
    glClearColor(environment.LightColor.x, environment.LightColor.y, environment.LightColor.z, 1.0f);

    auto dropped = new DroppedItem(2);
    Entities.push_back(std::unique_ptr<Entity>(dropped));
    dropped->pos_y = 40;
}

void Game::ProcessInput()
{
    if (KeyDown[GLFW_KEY_LEFT_ALT])
    {
        if (Freecam)
        {
            camera.Yaw = -90.0f;
            camera.Pitch = 0;
        }
        Freecam = !Freecam;
    }

    //TODO: seperate camera controller for freecam
    //camera.Position += glm::normalize(glm::vec3(camera.Right.x, 0.f, camera.Right.z)) * movement.x * (speed * Time::delta);
    //camera.Position += glm::normalize(glm::vec3(camera.Front.x, 0.f, camera.Front.z)) * movement.z * (speed * Time::delta);
    //camera.Position += glm::vec3(0.f, 1.f, 0.f) * movement.y * (speed * Time::delta);
    //camera.Yaw += lookScale * CursorDelta.x;
    //camera.Pitch -= lookScale * CursorDelta.y;
    //camera.Pitch = fmin(89.9f, fmax(-89.9f, camera.Pitch));

    static bool remove;
    static int layer;
    static bool drag;
    static bool draw;
    static glm::ivec2 start{};
    if (MouseDown[GLFW_MOUSE_BUTTON_LEFT])
    {
        auto block = worldGen.WorldToBlock(worldGen.CollideLayer(camera.ScreenToWorldPoint(CursorPosition)));
        if (KeyHeld[GLFW_KEY_LEFT_ALT])
        {
            drag = true;
            start = block;
        }
        else
            draw = KeyHeld[GLFW_KEY_LEFT_CONTROL];

        if (worldGen.GetBlock(block, 1))
        {
            remove = true;
            layer = 1;
        }
        else if (KeyHeld[GLFW_KEY_LEFT_SHIFT])
        {
            remove = true;
            layer = 0;
        }
        else
        {
            remove = false;
        }
        if (!(drag || draw))
        {
            if (remove)
            {
                if (layer) worldGen.RemoveBlock(block);
                else worldGen.RemoveBack(block);
            }
            else worldGen.PlaceBlock(block, 4);
        }
    }
    if (MouseHeld[GLFW_MOUSE_BUTTON_LEFT])
    {
        if (KeyHeld[GLFW_KEY_LEFT_CONTROL])
        {
            auto block = worldGen.WorldToBlock(worldGen.CollideLayer(camera.ScreenToWorldPoint(CursorPosition)));

            if (remove)
            {
                if (layer) worldGen.RemoveBlock(block);
                else worldGen.RemoveBack(block);
            }
            else worldGen.PlaceBlock(block, 4);
        }
    }
    if (MouseReleased[GLFW_MOUSE_BUTTON_LEFT])
    {
        if (drag)
        {
            auto block = worldGen.WorldToBlock(worldGen.CollideLayer(camera.ScreenToWorldPoint(CursorPosition)));
            glm::ivec2 minBlock(std::min(start.x, block.x), std::min(start.y, block.y));
            glm::ivec2 maxBlock(std::max(start.x, block.x), std::max(start.y, block.y));
            for (int y = maxBlock.y; y >= minBlock.y; y--)
            {
                for (int x = minBlock.x; x <= maxBlock.x; x++)
                {
                    if (remove)
                    {
                        if (layer) worldGen.RemoveBlock(x, y);
                        else worldGen.RemoveBack(x, y);
                    }
                    else worldGen.PlaceBlock(x, y, 4);
                }
            }
        }

        drag = false;
        draw = false;
    }
    if (MouseDown[GLFW_MOUSE_BUTTON_RIGHT])
    {
        cameraControl.StartDrag();
    }
    else if (MouseHeld[GLFW_MOUSE_BUTTON_RIGHT])
    {
        cameraControl.Drag();
    }
    if (ScrollY != 0)
    {
        cameraControl.Move(ScrollY > 0 ? true : false);
    }
}

void Game::Update()
{
    camera.Update();
    cameraControl.Update();

    for (auto& ent : Entities)
    {
        ent->Update();
        ent->floatPos.x = fmod(ent->floatPos.x + Time::delta + 5.0f, 10.0f) - 5.0f;
    }
    environment.LightColor -= glm::vec3(Time::delta * 0.2f * KeyHeld[GLFW_KEY_DELETE], Time::delta * 0.2f * KeyHeld[GLFW_KEY_END], Time::delta * 0.2f * KeyHeld[GLFW_KEY_PAGE_DOWN]) * (1.0f - 2.0f * KeyHeld[GLFW_KEY_LEFT_SHIFT]);
    glClearColor(environment.LightColor.x, environment.LightColor.y, environment.LightColor.z, 1.0f);
}

void Game::Render()
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = camera.GetProjectionMatrix();;
    glm::mat4 view = camera.GetViewMatrix();

    float time = glfwGetTime();

    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    shader->setInt("TileMap", 0);
    shader->setVec3("lightColor", environment.LightColor);
    TileMap.Bind();

    worldGen.Render();
    static float timer = 0.0f;
    timer += Time::delta;
    for (auto& ent : Entities)
    {
        ent->Render();
    }
}
