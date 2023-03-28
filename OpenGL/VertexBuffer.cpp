#include "VertexBuffer.hpp"
#include "Utils.hpp"

//---------------------------------

VertexBuffer::VertexBuffer():
	m_vbo_handle(0),
	m_vertices(),
	m_vertex_count(0),
	m_usage(Usage::StaticDraw)
{
	glGenBuffers(1, &m_vbo_handle);
	glcheck;
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_vbo_handle);
	glcheck;
}

//---------------------------------

void VertexBuffer::setUsage(VertexBuffer::Usage usage)
{
	m_usage = usage;
}

VertexBuffer::Usage VertexBuffer::getUsage() const
{
	return m_usage;
}

//---------------------------------

GLuint VertexBuffer::getNativeHandle() const
{
	return m_vbo_handle;
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_handle);
	glcheck;
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glcheck;
}

//---------------------------------

void VertexBuffer::addVertex(const Vertex& vertex)
{
	m_vertices.push_back(vertex);
}

void VertexBuffer::operator+=(const Vertex& vertex)
{
	addVertex(vertex);
}

//---------------------------------

size_t VertexBuffer::getVertexCount() const
{
	return m_vertex_count;
}

void VertexBuffer::commit()
{
	bind();
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof Vertex, m_vertices.data(), m_usage);
	m_vertex_count = m_vertices.size();
	m_vertices.clear();
	Unbind();
	glcheck;
}

void VertexBuffer::clear()
{
	m_vertices.clear();
}

//---------------------------------