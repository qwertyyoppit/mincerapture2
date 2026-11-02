#pragma once
#include <glm/glm.hpp>
#include "Camera.h"

class CameraController
{
public:
	CameraController(Camera &camera) : _camera(camera)
	{
		Distance = (int)_camera.Position.z;
	}
	float Speed = 5.0f;
	float Decelerate = 20.0f;
	int Distance;
	void Update();
	void StartDrag();
	void Drag();
	void Move(bool forward);

private:
	Camera &_camera;
	glm::vec2 Velocity{};
};