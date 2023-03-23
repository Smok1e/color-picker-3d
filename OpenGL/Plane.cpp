#include "Plane.hpp"

//---------------------------------

Plane::Plane():
	Primitive(),
	m_size(1, 1)
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

	Vertex vertices[4] = {
		glm::vec3(-m_size.x/2, 0, -m_size.y/2),
		glm::vec3(-m_size.x/2, 0,  m_size.y/2),
		glm::vec3( m_size.x/2, 0, -m_size.y/2),
		glm::vec3( m_size.x/2, 0,  m_size.y/2),
	};

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof Vertex, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//---------------------------------