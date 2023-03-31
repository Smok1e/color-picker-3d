#include "VertexBuffer.hpp"
#include "Utils.hpp"
#include "Logging.hpp"

//---------------------------------

VertexBuffer::VertexBuffer():
	m_vbo_handle(0),
	m_vertices(),
	m_vertex_count(0),
	m_usage(Usage::StaticDraw)
{
	glSafeCallVoid(glGenBuffers(1, &m_vbo_handle));
}

VertexBuffer::~VertexBuffer()
{
	glSafeCallVoid(glDeleteBuffers(1, &m_vbo_handle));
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
	glSafeCallVoid(glBindBuffer(GL_ARRAY_BUFFER, m_vbo_handle));
}

void VertexBuffer::Unbind()
{
	glSafeCallVoid(glBindBuffer(GL_ARRAY_BUFFER, 0));
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

void VertexBuffer::commit(bool calculate_normals /*= true*/)
{
	bind();
	if (calculate_normals) calculateNormals();
	glSafeCallVoid(glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof Vertex, m_vertices.data(), static_cast<GLenum>(m_usage)));
	m_vertex_count = m_vertices.size();
	clear();
	Unbind();
}

void VertexBuffer::clear()
{
	m_vertices.clear();
}

//---------------------------------

void VertexBuffer::calculateNormals()
{
	for (size_t i = 0, count = size()/3; i < count; i++)
	{
		iterator face = begin()+i*3;
		glm::vec3 edge1 = face[1].position-face[0].position;
		glm::vec3 edge2 = face[2].position-face[0].position;
		face[0].normal = face[1].normal = face[2].normal = glm::normalize(glm::cross(edge1, edge2));
	}
}

//---------------------------------

size_t VertexBuffer::size() const
{
	return m_vertices.size();
}

VertexBuffer::iterator VertexBuffer::begin()
{
	return m_vertices.begin();
}

VertexBuffer::iterator VertexBuffer::end()
{
	return m_vertices.end();
}

VertexBuffer::const_iterator VertexBuffer::begin() const
{
	return m_vertices.begin();
}

VertexBuffer::const_iterator VertexBuffer::end() const
{
	return m_vertices.end();
}

//---------------------------------