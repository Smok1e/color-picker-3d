#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Color.hpp"

//---------------------------------

#pragma pack(push, 1)
class Vertex
{
public:
	Vertex();
	Vertex(const Vertex& copy);
	Vertex(const glm::vec3& position, const glm::vec3& normal = glm::vec3(0, 0, 0), const glm::vec2& texcoord = glm::vec2(0, 0));

	static void InitAttributes();

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;

	constexpr float& operator[](size_t index) {
		return position[index];
	}

	constexpr float operator[](size_t index) const {
		return position[index];
	}
};
#pragma pack(pop)

//---------------------------------