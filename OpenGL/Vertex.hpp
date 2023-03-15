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
	Vertex(const glm::vec3& position, const Color& color);
	Vertex(const glm::vec3& position, const glm::vec3& normal, const Color& color);

	glm::vec3 position;
	glm::vec3 normal;
	Color color;
};
#pragma pack(pop)

//---------------------------------