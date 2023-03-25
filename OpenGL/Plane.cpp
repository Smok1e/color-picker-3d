#include "Plane.hpp"

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

void Plane::draw(Shader* shader /*= nullptr*/) const
{
	Primitive::draw(shader);
    glBindVertexArray(m_vertex_array);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);	
}

//---------------------------------

void Plane::updateVertexData()
{
	cleanup();

	glm::vec3 a(-m_size.x/2, 0, -m_size.y/2);
	glm::vec3 b(-m_size.x/2, 0,  m_size.y/2);
	glm::vec3 c( m_size.x/2, 0, -m_size.y/2);
	glm::vec3 d( m_size.x/2, 0,  m_size.y/2);

	glm::vec3 normal = glm::cross(b-a, c-b);

	Vertex vertices[4] = {
		Vertex(a, normal, glm::vec2(0, 0)),
		Vertex(b, normal, glm::vec2(0, 1)),
		Vertex(c, normal, glm::vec2(1, 0)),
		Vertex(d, normal, glm::vec2(1, 1)),
	};

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof Vertex, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	Vertex::InitAttributes();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//---------------------------------