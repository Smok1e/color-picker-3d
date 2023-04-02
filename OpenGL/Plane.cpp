#include "Plane.hpp"
#include "DebugVisualization.hpp"

//---------------------------------

Plane::Plane():
	Primitive(),
	m_size(glm::vec2(1, 1))
{
	updateVertexData();
}

//---------------------------------

void Plane::setSize(glm::vec2 size)
{
	if (m_size != size)
		m_size = size, updateVertexData();
}

glm::vec2 Plane::getSize() const
{
	return m_size;
}

//---------------------------------

void Plane::calculateVertices()
{
    // a-----b
    // |    /|
    // |   / |
    // |  /  |
    // | /   |
    // |/    |
    // c-----d

	Vertex a(glm::vec3(-m_size.x/2, 0, -m_size.y/2), glm::vec2(0, 0));
	Vertex b(glm::vec3( m_size.x/2, 0, -m_size.y/2), glm::vec2(1, 0));
	Vertex c(glm::vec3(-m_size.x/2, 0,  m_size.y/2), glm::vec2(0, 1));
	Vertex d(glm::vec3( m_size.x/2, 0,  m_size.y/2), glm::vec2(1, 1));

	m_vertex_buffer += c;
	m_vertex_buffer += b;
	m_vertex_buffer += a;

	m_vertex_buffer += b;
	m_vertex_buffer += c;
	m_vertex_buffer += d;

	m_vertex_buffer.commit();
}

//---------------------------------