#include "Vertex.hpp"

//---------------------------------

Vertex::Vertex():
	position (0),
	texcoord (0),
	normal   (0),
	tangent  (0),
	bitangent(0)
{}

Vertex::Vertex(const Vertex& copy):
	position(copy.position),
	texcoord(copy.texcoord),
	normal(copy.normal),
	tangent(copy.tangent),
	bitangent(copy.bitangent)
{}

Vertex::Vertex(
	const glm::vec3& position, 
	const glm::vec2& texcoord  /*= glm::vec2(0)*/, 
	const glm::vec3& normal    /*= glm::vec3(0)*/, 
	const glm::vec3& tangent   /*= glm::vec3(0)*/,
	const glm::vec3& bitangent /*= glm::vec3(0)*/
):
	position(position),
	texcoord(texcoord),
	normal(normal),
	tangent(tangent),
	bitangent(bitangent)
{}

//---------------------------------

void Vertex::InitAttributes()
{
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, nullptr);
	glEnableVertexAttribArray(0);

	// UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof Vertex, reinterpret_cast<void*>(sizeof Vertex::position));
	glEnableVertexAttribArray(1);

	// Normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, reinterpret_cast<void*>(sizeof Vertex::position + sizeof Vertex::texcoord));
	glEnableVertexAttribArray(2);

	// Tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, reinterpret_cast<void*>(sizeof Vertex::position + sizeof Vertex::texcoord + sizeof Vertex::normal));
	glEnableVertexAttribArray(3);

	// Bitangent
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, reinterpret_cast<void*>(sizeof Vertex::position + sizeof Vertex::texcoord + sizeof Vertex::normal + sizeof Vertex::tangent));
	glEnableVertexAttribArray(4);
}

//---------------------------------