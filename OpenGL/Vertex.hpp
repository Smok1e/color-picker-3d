#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Color.hpp"

//---------------------------------

#pragma pack(push, 1)
#pragma warning(disable: 4267)

class Vertex
{
public:
	Vertex();
	Vertex(const Vertex& copy);
	Vertex(
		const glm::vec3& position, 
		const glm::vec2& texcoord  = glm::vec2(0), 
		const glm::vec3& normal    = glm::vec3(0), 
		const glm::vec3& tangent   = glm::vec3(0)
	);

	static void InitAttributes();

	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
	glm::vec3 tangent;

	constexpr float& operator[](size_t index) {
		return position[index];
	}

	constexpr float operator[](size_t index) const {
		return position[index];
	}
};

#pragma pack(pop)

//---------------------------------
