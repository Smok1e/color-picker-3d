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

		   vertices.push_back(glm::vec3(x*zr0, y*zr0, z0)*m_radius);

           indices.push_back(index);
		   index++;

		   vertices.push_back(glm::vec3(x*zr1, y*zr1, z1)*m_radius);

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    glGenBuffers(1, &m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);	
	m_index_count = indices.size();
}

//---------------------------------

#if 0
#define _USE_MATH_DEFINES

#include <cmath>
#include <glm/gtx/color_space.hpp>

#include "Sphere.hpp"
#include "Color.hpp"
#include "Utils.hpp"
#include "Vertex.hpp"

//---------------------------------

Sphere::Sphere():
	m_vertex_buffer(0),
	m_vertex_array(0),
	m_index_buffer(0),
	m_index_count(0),
	m_position(0),
	m_latitude_points(16),
	m_longitude_points(16),
	m_radius(.5f),
	m_color(Color::White)
{
	/*
	constexpr size_t points_count = 128;
	constexpr size_t vertex_count = points_count + 2;
	constexpr float radius = .5f;
	constexpr float z = 0;

	Vertex vertices[vertex_count] = {};

	// Central vertex
	vertices[0] = Vertex(0, 0, z, 1, 1, 1);

	for (size_t point = 0; point < points_count; point++)
	{
		double angle = (M_PI * 2 / points_count) * point;
		vertices[point+1] = Vertex(glm::vec3(cos(angle), sin(angle), z)*radius, Color::HSV((255.0 / points_count) * point, 255, 255));
	}

	// Closing vertex
	vertices[points_count+1] = Vertex(glm::vec3(cos(0)*radius, sin(0)*radius, z), Color::HSV(255, 255, 255));

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, nullptr);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof Vertex, reinterpret_cast<void*>(3 * sizeof GLfloat));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	*/

	updateVertexData();
}

Sphere::~Sphere()
{
	destroy();
}

//---------------------------------

void Sphere::updateVertexData()
{
	destroy();

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

		   vertices.push_back(glm::vec3(x*zr0, y*zr0, z0)*m_radius);

           indices.push_back(index);
		   index++;

		   vertices.push_back(glm::vec3(x*zr1, y*zr1, z1)*m_radius);

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    glGenBuffers(1, &m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);	
	m_index_count = indices.size();
}

void Sphere::destroy()
{
	if (m_vertex_buffer) glDeleteBuffers(1, &m_vertex_buffer);
	if (m_vertex_array) glDeleteVertexArrays(1, &m_vertex_array);
	if (m_index_buffer) glDeleteBuffers(1, &m_index_buffer);	
	m_vertex_buffer = m_vertex_array = m_index_buffer = 0;
}

//---------------------------------

void Sphere::draw(Shader* shader /*= nullptr*/)
{
	if (shader)
	{
		shader -> setUniform("shapeColor",  m_color   );
		shader -> setUniform("shapeOffset", m_position);
		shader -> use();
	}

    glBindVertexArray(m_vertex_array);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glDrawElements(GL_QUAD_STRIP, m_index_count, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//---------------------------------

void Sphere::setPosition(const glm::vec3& position)
{
	m_position = position;
}

glm::vec3 Sphere::getPosition() const
{
	return m_position;
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

void Sphere::setRadius(float radius)
{
	if (radius != m_radius)
		m_radius = radius, updateVertexData();
}

float Sphere::getRadius() const
{
	return m_radius;
}

void Sphere::setColor(const Color& color)
{
	m_color = color;
}

Color Sphere::getColor() const
{
	return m_color;
}

//---------------------------------
#endif