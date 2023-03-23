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
	Vertex(const glm::vec3& position);

	glm::vec3 position;

	constexpr float& operator[](size_t index) {
		return position[index];
	}

	constexpr float operator[](size_t index) const {
		return position[index];
	}
};
#pragma pack(pop)

//---------------------------------