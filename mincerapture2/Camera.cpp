#include "Camera.h"
#include "Game.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float pitch, float yaw)
{
    Position = position;
    WorldUp = up;
    Pitch = pitch;
    Yaw = yaw;
    Update();
}

Camera::Camera(glm::vec3 position, glm::vec3 up)
{
    Position = position;
    WorldUp = up;
    Update();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float pitch, float yaw)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Pitch = pitch;
    Yaw = yaw;
    Update();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Update();
}

Camera::Camera()
{
    if (!main) main = this;
    Update();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(glm::radians(FieldOfView), (float)Blockheads.Width / (float)Blockheads.Height, Near, Far);
}

std::tuple<glm::vec3, glm::vec3> Camera::ScreenToWorldPoint(const glm::vec2& pos) const
{
    glm::mat4 invMat = glm::inverse(GetProjectionMatrix() * GetViewMatrix());
    glm::vec4 near ((pos.x - Blockheads.Width / 2) / (Blockheads.Width / 2), -1 * (pos.y - Blockheads.Height / 2) / (Blockheads.Height / 2), -1, 1.0);
    glm::vec4 far ((pos.x - Blockheads.Width / 2) / (Blockheads.Width / 2), -1 * (pos.y - Blockheads.Height / 2) / (Blockheads.Height / 2), 1, 1.0);
    glm::vec4 nearResult = invMat * near;
    glm::vec4 farResult = invMat * far;
    nearResult /= nearResult.w;
    farResult /= farResult.w;
    glm::vec3 dir (farResult - nearResult);
    return { Position, glm::normalize(dir) };
}

void Camera::Update()
{
    // calculate the new Front vector
    glm::vec3 front{};
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
    _front = Front;
    _right = Right;
    _up = Up;
}