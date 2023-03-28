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
	glSafeCallVoid(glBindVertexArray(m_vertex_array));
	Primitive::draw(shader);
	glSafeCallVoid(glDrawArrays(GL_TRIANGLES, 0, m_vertex_buffer.getVertexCount()));
	glSafeCallVoid(glBindVertexArray(0));
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

	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 0), glm::vec3( 0,  0, -1));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0), glm::vec3( 0,  0, -1));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(0, 1), glm::vec3( 0,  0, -1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 0), glm::vec3( 0,  0, -1));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(0, 1), glm::vec3( 0,  0, -1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5, -0.5), glm::vec2(1, 1), glm::vec3( 0,  0, -1));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(1, 0), glm::vec3(-1,  0,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0), glm::vec3(-1,  0,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(0, 1), glm::vec3(-1,  0,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(1, 0), glm::vec3(-1,  0,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(0, 1), glm::vec3(-1,  0,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5,  0.5), glm::vec2(1, 1), glm::vec3(-1,  0,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(1, 0), glm::vec3( 0,  0,  1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5,  0.5), glm::vec2(0, 0), glm::vec3( 0,  0,  1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3( 0,  0,  1));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(1, 0), glm::vec3( 0,  0,  1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3( 0,  0,  1));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5,  0.5), glm::vec2(1, 1), glm::vec3( 0,  0,  1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 0), glm::vec3( 1,  0,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5, -0.5), glm::vec2(0, 0), glm::vec3( 1,  0,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3( 1,  0,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 0), glm::vec3( 1,  0,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3( 1,  0,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5,  0.5), glm::vec2(1, 1), glm::vec3( 1,  0,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5, -0.5), glm::vec2(1, 0), glm::vec3( 0,  1,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(0, 0), glm::vec3( 0,  1,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3( 0,  1,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5, -0.5), glm::vec2(1, 0), glm::vec3( 0,  1,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3( 0,  1,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(1, 1), glm::vec3( 0,  1,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5,  0.5), glm::vec2(1, 0), glm::vec3( 0, -1,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(0, 0), glm::vec3( 0, -1,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 1), glm::vec3( 0, -1,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5,  0.5), glm::vec2(1, 0), glm::vec3( 0, -1,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 1), glm::vec3( 0, -1,  0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 1), glm::vec3( 0, -1,  0));

	m_vertex_buffer.commit();
	m_vertex_buffer.bind();

	glSafeCallVoid(glGenVertexArrays(1, &m_vertex_array));
	glSafeCallVoid(glBindVertexArray(m_vertex_array));
	Vertex::InitAttributes();
	VertexBuffer::Unbind();
	glSafeCallVoid(glBindVertexArray(0));
}

//---------------------------------