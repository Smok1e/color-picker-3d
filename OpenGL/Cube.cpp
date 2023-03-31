#include "Cube.hpp"

//---------------------------------

Cube::Cube():
	Primitive(),
	m_size(1, 1, 1)
{
	updateVertexData();
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

void Cube::calculateVertices()
{
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(0, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(0, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5, -0.5), glm::vec2(1, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(1, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(0, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(1, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(0, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5,  0.5), glm::vec2(1, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(1, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5,  0.5), glm::vec2(0, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(0, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(1, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(0, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5,  0.5), glm::vec2(1, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5, -0.5), glm::vec2(0, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(0, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(0, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5,  0.5), glm::vec2(1, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5, -0.5), glm::vec2(1, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5, -0.5), glm::vec2(0, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5,  0.5), glm::vec2(0, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5, -0.5), glm::vec2(1, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5,  0.5,  0.5), glm::vec2(0, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5,  0.5,  0.5), glm::vec2(1, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5,  0.5), glm::vec2(1, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5,  0.5), glm::vec2(0, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5,  0.5), glm::vec2(1, 0));
	m_vertex_buffer += Vertex(m_size*glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 1));
	m_vertex_buffer += Vertex(m_size*glm::vec3( 0.5, -0.5, -0.5), glm::vec2(1, 1));
	m_vertex_buffer.commit();
}

//---------------------------------