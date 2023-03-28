#include "Cube.hpp"

//---------------------------------

Cube::Cube():
	Primitive(),
	m_size(1, 1, 1)
{
	updateVertexData();
}

//---------------------------------

void Cube::draw(Shader* shader /*= nullptr*/) const
{
	glBindVertexArray(m_vertex_array);
	Primitive::draw(shader);
	glDrawArrays(GL_TRIANGLES, 0, m_vertex_count);
	glBindVertexArray(0);
}

//---------------------------------

void Cube::setSize(const glm::vec3& size)
{
	if (m_size != size)
		m_size = size, updateVertexData();
}

glm::vec3 Cube::getSize() const
{
	return m_size;
}

//---------------------------------

void Cube::updateVertexData()
{
	cleanup();

	Vertex vertices[] = {
		Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 0), glm::vec3( 0,  0, -1)),
		Vertex(m_size*glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0), glm::vec3( 0,  0, -1)),
		Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(0, 1), glm::vec3( 0,  0, -1)),
		Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 0), glm::vec3( 0,  0, -1)),
		Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(0, 1), glm::vec3( 0,  0, -1)),
		Vertex(m_size*glm::vec3( 0.5,  0.5, -0.5), glm::vec2(1, 1), glm::vec3( 0,  0, -1)),

		Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(1, 0), glm::vec3(-1,  0,  0)),
		Vertex(m_size*glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0), glm::vec3(-1,  0,  0)),
		Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(0, 1), glm::vec3(-1,  0,  0)),
		Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(1, 0), glm::vec3(-1,  0,  0)),
		Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(0, 1), glm::vec3(-1,  0,  0)),
		Vertex(m_size*glm::vec3(-0.5,  0.5,  0.5), glm::vec2(1, 1), glm::vec3(-1,  0,  0)),

		Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(1, 0), glm::vec3( 0,  0,  1)),
		Vertex(m_size*glm::vec3( 0.5, -0.5,  0.5), glm::vec2(0, 0), glm::vec3( 0,  0,  1)),
		Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3( 0,  0,  1)),
		Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(1, 0), glm::vec3( 0,  0,  1)),
		Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3( 0,  0,  1)),
		Vertex(m_size*glm::vec3(-0.5,  0.5,  0.5), glm::vec2(1, 1), glm::vec3( 0,  0,  1)),

		Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 0), glm::vec3( 1,  0,  0)),
		Vertex(m_size*glm::vec3( 0.5,  0.5, -0.5), glm::vec2(0, 0), glm::vec3( 1,  0,  0)),
		Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3( 1,  0,  0)),
		Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 0), glm::vec3( 1,  0,  0)),
		Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3( 1,  0,  0)),
		Vertex(m_size*glm::vec3( 0.5, -0.5,  0.5), glm::vec2(1, 1), glm::vec3( 1,  0,  0)),

		Vertex(m_size*glm::vec3( 0.5,  0.5, -0.5), glm::vec2(1, 0), glm::vec3( 0,  1,  0)),
		Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(0, 0), glm::vec3( 0,  1,  0)),
		Vertex(m_size*glm::vec3(-0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3( 0,  1,  0)),
		Vertex(m_size*glm::vec3( 0.5,  0.5, -0.5), glm::vec2(1, 0), glm::vec3( 0,  1,  0)),
		Vertex(m_size*glm::vec3(-0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3( 0,  1,  0)),
		Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(1, 1), glm::vec3( 0,  1,  0)),

		Vertex(m_size*glm::vec3( 0.5, -0.5,  0.5), glm::vec2(1, 0), glm::vec3( 0, -1,  0)),
		Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(0, 0), glm::vec3( 0, -1,  0)),
		Vertex(m_size*glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 1), glm::vec3( 0, -1,  0)),
		Vertex(m_size*glm::vec3( 0.5, -0.5,  0.5), glm::vec2(1, 0), glm::vec3( 0, -1,  0)),
		Vertex(m_size*glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 1), glm::vec3( 0, -1,  0)),
		Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 1), glm::vec3( 0, -1,  0))
	};

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	Vertex::InitAttributes();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_vertex_count = sizeof vertices/sizeof Vertex;
}

//---------------------------------