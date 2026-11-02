#pragma once
#include <glm/glm.hpp>
#include "WorldGen.h" // THIS IS FINE!!!! THANK GOD!!! BECAUSE Game.h is only included in file.cpp yay!!!!
#include "Environment.h"
//TODO: Add "Game.h" to a common include header

class Game
{
public:
	unsigned int Width;
	unsigned int Height;

	bool KeyHeld[1024]{};
	bool KeyDown[1024]{};
	bool KeyReleased[1024]{};
	bool MouseHeld[8]{};
	bool MouseDown[8]{};
	bool MouseReleased[8]{};
	glm::vec2 CursorPosition{};
	glm::vec2 CursorDelta{};
	float ScrollY{};

	WorldGen worldGen;
	Environment environment = { .LightDirection = glm::normalize(glm::vec3(1.0, -2.0, -1.0)), .LightColor = glm::vec3(0.75f, 0.85f, 1.0f)};

	Game(unsigned int width, unsigned int height);
	~Game();

	void Init();

	void ProcessInput();

	void Update();

	void Render();
};

extern Game Blockheads;