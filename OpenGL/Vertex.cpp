#include "Vertex.hpp"

//---------------------------------

Vertex::Vertex():
	position(),
	normal(),
	color()
{}

Vertex::Vertex(const Vertex& copy):
	position(copy.position),
	normal(copy.normal),
	color(copy.color)
{}

Vertex::Vertex(const glm::vec3& position):
	position(position),
	normal(),
	color()
{}

Vertex::Vertex(const glm::vec3& position, const Color& color):
	position(position),
	normal(),
	color(color)
{}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const Color& color):
	position(position),
	normal(normal),
	color(color)
{}

//---------------------------------