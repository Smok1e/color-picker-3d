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

	glm::vec2 uv_a(0, 0);
	glm::vec2 uv_b(0, 1);
	glm::vec2 uv_c(1, 0);
	glm::vec2 uv_d(1, 1);

	glm::vec3 edge1 = b-a;
	glm::vec3 edge2 = c-a;
	glm::vec2 delta_uv1 = uv_b-uv_a;
	glm::vec2 delta_uv2 = uv_c-uv_a;

	float f = 1.f / (delta_uv1.x*delta_uv2.y - delta_uv2.x*delta_uv1.y);
	glm::vec3 tangent = f*glm::vec3(
		delta_uv2.y * edge1.x - delta_uv1.y * edge2.x,
		delta_uv2.y * edge1.y - delta_uv1.y * edge2.y,
		delta_uv2.y * edge1.z - delta_uv1.y * edge2.z
	);

	glm::vec3 bitangent = f*glm::vec3(
		-delta_uv2.x * edge1.x + delta_uv1.x * edge2.x,
		-delta_uv2.x * edge1.y + delta_uv1.x * edge2.y,
		-delta_uv2.x * edge1.z + delta_uv1.x * edge2.z
	);

	glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

	Vertex vertices[4] = {
		Vertex(a, uv_a, normal, tangent, bitangent),
		Vertex(b, uv_b, normal, tangent, bitangent),
		Vertex(c, uv_c, normal, tangent, bitangent),
		Vertex(d, uv_d, normal, tangent, bitangent)
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