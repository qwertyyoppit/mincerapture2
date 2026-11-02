#pragma once
#include <vector>
#include "Entity.h"
#include "includes/shader.h"
#include "BlockIDs.h"
#include "Texture.h"
#include "LoadTextureFromFile.h"

class DroppedItem : public Entity
{
public:
	//Item item;

	DroppedItem(unsigned short id);

	void Update();

	void Render();

	glm::vec2 velocity = { 0.0f, 1.0f };

private:
	unsigned int VBO, VAO, EBO;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;
	};

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Shader itemShader = Shader("DroppedItem.vert", "DroppedItem.frag");
	Texture2D texture = LoadTextureFromFile("Tilemap.png", true);

	const float terminalVelocity = 20.0f;
};