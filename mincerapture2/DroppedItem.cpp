#include "DroppedItem.h"
#include "Time.h"
#include "BlockRegistry.h"
#include "FaceVertices.h"
#include "Faces.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Constants.h"
#include "Game.h"

DroppedItem::DroppedItem(unsigned short id)
{
	auto& bd = BlockRegistry::GetBlock(id);

	{
		int face = 0;
		for (auto& fd : FaceDatas)
		{
			auto faceuv = bd.GetUVs(face);
			for (int i = 0; i < fd.Vertices.size(); i++)
			{
				vertices.push_back(Vertex(fd.Vertices[i] - glm::vec3(0.5f), fd.Normal, faceuv[i]));
			}
			for (auto indice : FaceIndices)
			{
				indices.push_back(vertices.size() - 4 + indice);
			}
			
			face++;
		}
	}

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
	glEnableVertexAttribArray(2);
}

void DroppedItem::Update()
{
	floatPos += velocity * (Time::delta);
	velocity.y -= 9.8f * Time::delta;
	velocity.y = fmin(terminalVelocity, fmax(velocity.y, -terminalVelocity));
	auto pos = glm::vec2(floatPos.x, floatPos.y) + glm::vec2(pos_x, pos_y);
	for (auto& point : { glm::vec2(0.0f, -0.5f), glm::vec2(0.0f, 0.5f) })
	{
		auto block = WorldGen::WorldToBlock(pos + point);
		if (Blockheads.worldGen.GetBlock(block, 1))
		{
			floatPos.y = 0.5f;
			pos_y = block.y - copysignf(1, point.y);
			velocity.y = 0;
			break;
		}
	}
}

void DroppedItem::Render()
{
	itemShader.use();

	glm::mat4 model = (1.0f);
	model = glm::translate(model, glm::vec3(floatPos.x + pos_x, floatPos.y + pos_y, WORLD_FRONT_Z-1));
	model = glm::translate(model, glm::vec3(0, 0, 0.5f));
	model = glm::rotate(model, glm::pi<float>() * Time::time / 2, glm::vec3(0.0, 1.0, 0.0));
	model = glm::scale(model, glm::vec3(0.8f));
	Camera& cam = *Camera::main;
	auto view = cam.GetViewMatrix();
	auto projection = cam.GetProjectionMatrix();

	itemShader.setMat4("model", model);
	itemShader.setMat4("view", view);
	itemShader.setMat4("projection", projection);
	itemShader.setVec3("lightColor", Blockheads.environment.LightColor);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}