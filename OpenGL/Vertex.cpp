#include "Vertex.hpp"

//---------------------------------

Vertex::Vertex():
	position()
{}

Vertex::Vertex(const Vertex& copy):
	position(copy.position)
{}

Vertex::Vertex(const glm::vec3& position):
	position(position)
{}

//---------------------------------