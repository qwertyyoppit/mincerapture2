#pragma once
#include <glm/glm.hpp>
#include <tuple>

class Camera
{
public:
    glm::vec3 Position{ 0.0f };
    glm::vec3 WorldUp{ 0.0f, 1.0f, 0.0f };
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;

    float Yaw = -90.0f;
    float Pitch = 0.0f;

    float FieldOfView = 60.0f;
    float Near = 0.1f;
    float Far = 200.0f;

    Camera(glm::vec3 position, glm::vec3 up, float pitch, float yaw);

    Camera(glm::vec3 position, glm::vec3 up);
    
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float pitch, float yaw);

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ);

    Camera();

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const;

    glm::mat4 GetProjectionMatrix() const;

    std::tuple<glm::vec3, glm::vec3> ScreenToWorldPoint(const glm::vec2& pos) const;
    
    void Update();

    inline static Camera* main;
private:
    glm::vec3 _front;
    glm::vec3 _right;
    glm::vec3 _up;
};