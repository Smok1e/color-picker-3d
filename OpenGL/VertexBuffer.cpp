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
		glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
		
		glm::vec2 delta_uv1 = face[1].texcoord - face[0].texcoord;
		glm::vec2 delta_uv2 = face[2].texcoord - face[0].texcoord;  
		
		glm::vec3 tangent = 1.f / (delta_uv1.x*delta_uv2.y - delta_uv2.x*delta_uv1.y) * glm::vec3(
			delta_uv2.y*edge1.x - delta_uv1.y*edge2.x,
			delta_uv2.y*edge1.y - delta_uv1.y*edge2.y,
			delta_uv2.y*edge1.z - delta_uv1.y*edge2.z
		);
		
		face[0].normal  = face[1].normal  = face[2].normal  = normal;
		face[0].tangent = face[1].tangent = face[2].tangent = tangent;
		// Bitangent vector will be calculated later in the vertex 
		// shader as cross product of normal and tangent vectors
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