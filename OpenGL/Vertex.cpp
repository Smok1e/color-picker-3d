#include "Vertex.hpp"

//---------------------------------

Vertex::Vertex():
	position(),
	normal(),
	texcoord()
{}

Vertex::Vertex(const Vertex& copy):
	position(copy.position),
	normal(copy.normal),
	texcoord(copy.texcoord)
{}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texcoord):
	position(position),
	normal(normal),
	texcoord(texcoord)
{}

//---------------------------------

void Vertex::InitAttributes()
{
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, nullptr);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, reinterpret_cast<void*>(sizeof Vertex::position));
	glEnableVertexAttribArray(1);

	// Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof Vertex, reinterpret_cast<void*>(sizeof Vertex::position + sizeof Vertex::normal));
	glEnableVertexAttribArray(2);
}

//---------------------------------