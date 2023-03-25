#define _USE_MATH_DEFINES
#include <cmath>
#include "Sphere.hpp"

//---------------------------------

Sphere::Sphere():
	Primitive(),
	m_index_buffer(0),
	m_index_count(0),
	m_latitude_points(16),
	m_longitude_points(16),
	m_radius(.5f)
{
	updateVertexData();
}

//---------------------------------

void Sphere::draw(Shader* shader /*= nullptr*/) const
{
	Primitive::draw(shader);
    glBindVertexArray(m_vertex_array);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glDrawElements(GL_QUAD_STRIP, m_index_count, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//---------------------------------

void Sphere::setRadius(float radius)
{
	if (radius != m_radius)
		m_radius = radius, updateVertexData();
}

float Sphere::getRadius() const
{
	return m_radius;
}

void Sphere::setPointCount(size_t latitude, size_t longitude)
{
	if (latitude != m_latitude_points || longitude != m_longitude_points)
	{
		m_latitude_points = latitude;
		m_longitude_points = longitude;
		updateVertexData();
	}
}

size_t Sphere::getLatitudePointCount() const
{
	return m_latitude_points;
}

size_t Sphere::getLongitudePointCount() const
{
	return m_longitude_points;
}

//---------------------------------

void Sphere::cleanup()
{
	Primitive::cleanup();
	if (m_index_buffer) glDeleteBuffers(1, &m_index_buffer);
	m_index_buffer = 0;
}

//---------------------------------

void Sphere::updateVertexData()
{
	cleanup();

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	for (size_t lat = 1, index = 0; lat <= m_latitude_points; lat++)
	{
		double alpha0 = M_PI *(static_cast<double>(lat-1)/m_latitude_points - .5);
		double z0     = sin(alpha0);
		double zr0    = cos(alpha0);

		double alpha1 = M_PI * (static_cast<double>(lat)/m_latitude_points - .5);
		double z1     = sin(alpha1);
		double zr1    = cos(alpha1);

		for (size_t lng = 1; lng <= m_longitude_points+1; lng++) 
		{
           double beta = 2 * M_PI * static_cast<double>(lng-1) / m_longitude_points;
           double x = cos(beta);
           double y = sin(beta);
									 
		   glm::vec3 position = glm::vec3(x*zr0, y*zr0, z0)*m_radius;
		   glm::vec3 normal = glm::normalize(position);
		   glm::vec2 texcoord(atan2(normal.x, normal.z) / (2*M_PI) + 0.5, normal.y * 0.5 + 0.5);
		   vertices.push_back(Vertex(position, normal, texcoord));

           indices.push_back(index);
		   index++;

		   position = glm::vec3(x*zr1, y*zr1, z1)*m_radius;
		   normal = glm::normalize(position);
		   texcoord = glm::vec2(atan2(normal.x, normal.z) / (2*M_PI) + 0.5, normal.y * 0.5 + 0.5);
		   vertices.push_back(Vertex(position, normal, texcoord));

           indices.push_back(index);
		   index++;
        }

        indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	}

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof Vertex, vertices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	Vertex::InitAttributes();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    glGenBuffers(1, &m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);	
	m_index_count = indices.size();
}

//---------------------------------