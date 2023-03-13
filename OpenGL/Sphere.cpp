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
	m_position(0)
{
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
		vertices[point+1] = Vertex(glm::vec3(cos(angle)*radius, sin(angle)*radius, z), Color::HSV((255.0 / points_count) * point, 255, 255));
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
}

Sphere::~Sphere()
{
	glDeleteBuffers(1, &m_vertex_buffer);
	glDeleteVertexArrays(1, &m_vertex_array);
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

	GLint vertex_buffer_size = 0;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vertex_buffer_size);
	size_t vertex_count = vertex_buffer_size / sizeof GLfloat / 7;
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(m_vertex_array);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertex_count);
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

//---------------------------------