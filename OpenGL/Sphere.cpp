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
	m_position(0),
	m_latitude_points(16),
	m_longitude_points(16),
	m_radius(.5f)
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

	update();
}

Sphere::~Sphere()
{
	destroy();
}

//---------------------------------

void Sphere::update()
{
	destroy();

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	for (size_t lat = 1, indicator = 0; lat <= m_latitude_points; lat++)
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

		   Color color = Color::White; //Color::HSV(255*(static_cast<double>(lng)/m_longitude_points), 255, 255);

		   glm::vec3 position0 = glm::vec3(x*zr0, y*zr0, z0)*m_radius;
		   vertices.push_back(Vertex(
			   position0,
			   glm::normalize(position0),
			   color 
		   ));

           indices.push_back(indicator);
		   indicator++;

		   glm::vec3 position1 = glm::vec3(x*zr1, y*zr1, z1)*m_radius;
		   vertices.push_back(Vertex(
			   position1,
			   glm::normalize(position1),
			   color 
		   ));

           indices.push_back(indicator);
		   indicator++;
        }

        indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	}

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof Vertex, vertices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, reinterpret_cast<void*>(3*sizeof(float)));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof Vertex, reinterpret_cast<void*>(6*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    glGenBuffers(1, &m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);	
}

void Sphere::destroy()
{
	if (m_vertex_buffer) glDeleteBuffers(1, &m_vertex_buffer);
	if (m_vertex_array) glDeleteVertexArrays(1, &m_vertex_array);
	if (m_index_buffer) glDeleteBuffers(1, &m_index_buffer);	
	m_vertex_buffer = m_vertex_array = m_index_buffer = 0;
}

//---------------------------------

void Sphere::draw()
{
	GLint program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);
	if (program)
	{
		GLint location = glGetUniformLocation(program, "offset");
		if (location != -1)
			glUniform3f(location, m_position.x, m_position.y, m_position.z);
	}

	GLint index_buffer_size = 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &index_buffer_size);
	size_t index_count = index_buffer_size / sizeof GLuint;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // draw sphere
    glBindVertexArray(m_vertex_array);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glDrawElements(GL_QUAD_STRIP, index_count, GL_UNSIGNED_INT, NULL);
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
		update();
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
		m_radius = radius, update();
}

float Sphere::getRadius() const
{
	return m_radius;
}

//---------------------------------