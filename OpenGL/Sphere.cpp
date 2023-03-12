#define _USE_MATH_DEFINES

#include <cmath>
#include <glm/gtx/color_space.hpp>

#include "Sphere.hpp"
#include "Color.hpp"
#include "Utils.hpp"

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

	// Vertex: { x, y, z, r, g, b, a }
	constexpr size_t vertex_components = 7;

	GLfloat vertices[vertex_count * vertex_components] = {};
	for (size_t point = 0; point < points_count; point++)
	{
		GLfloat* vertex = vertices + vertex_components * (point + 1);
		glm::vec4 color = Color::HSV((255.0 / points_count) * point, 255, 255);
		double angle = (M_PI * 2 / points_count) * point;
		vertex[0] = cos(angle) * radius;
		vertex[1] = sin(angle) * radius;
		vertex[2] = z;
		vertex[3] = color.r;
		vertex[4] = color.g;
		vertex[5] = color.b;
		vertex[6] = color.a;
	}

	GLfloat* closing_vertex = vertices + vertex_components * (points_count + 1);
	glm::vec4 closing_vertex_color = Color::HSV(255, 255, 255);
	closing_vertex[0] = cos(0) * radius;
	closing_vertex[1] = sin(0) * radius;
	closing_vertex[2] = z;
	closing_vertex[3] = closing_vertex_color.r;
	closing_vertex[4] = closing_vertex_color.g;
	closing_vertex[5] = closing_vertex_color.b;
	closing_vertex[6] = closing_vertex_color.a;

	GLfloat* central_vertex = vertices;
	central_vertex[0] = 0.f;
	central_vertex[1] = 0.f;
	central_vertex[2] = z;
	central_vertex[3] = 1.f;
	central_vertex[4] = 1.f;
	central_vertex[5] = 1.f;
	central_vertex[6] = 1.f;

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_components * sizeof GLfloat, nullptr);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertex_components * sizeof GLfloat, reinterpret_cast<void*>(3 * sizeof GLfloat));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/*
	const float h_angle = M_PI / 180 * 72;
	const float v_angle = atan(.5);
	const float radius = .5;
	constexpr size_t vertex_count = 36;

	// Vertex: { x, y, z, r, g, b, a }
	constexpr size_t vertex_components = 7;
	GLfloat vertices[vertex_count * vertex_components] = {};

	float h_angle1 = -M_PI/2 - h_angle/2;
	float h_angle2 = -M_PI/2;

	GLfloat* top_vertex = vertices;
	top_vertex[0] = 0;
	top_vertex[1] = 0;
	top_vertex[2] = radius;
	top_vertex[3] = 1;
	top_vertex[4] = 1;
	top_vertex[5] = 1;
	top_vertex[6] = 1;

	// compute 10 vertices at 1st and 2nd rows
	for (int i = 1; i <= 5; i++)
	{
		GLfloat* vertex1 = vertices +  i   *vertex_components;
		GLfloat* vertex2 = vertices + (i+5)*vertex_components;

		float z  = radius * sin(h_angle); // elevaton
		float xy = radius * cos(h_angle); // length on XY plane

		vertex1[0] =  xy * cos(h_angle1);
		vertex1[1] =  xy * sin(h_angle1);
		vertex1[2] =  z;
		vertex1[3] = 1;
		vertex1[4] = 1;
		vertex1[5] = 1;
		vertex1[6] = 1;

		vertex2[0] =  xy * cos(h_angle2);
		vertex2[1] =  xy * sin(h_angle2);
		vertex2[2] = -z;
		vertex2[3] = 1;
		vertex2[4] = 1;
		vertex2[5] = 1;
		vertex2[6] = 1;

		// next horizontal angles
		h_angle1 += h_angle;
		h_angle2 += h_angle;
	}

	GLfloat* bottom_vertex = vertices + 11*vertex_components;
	bottom_vertex[0] = 0;
	bottom_vertex[1] = 0;
	bottom_vertex[2] = -radius;
	bottom_vertex[3] = 1;
	bottom_vertex[4] = 1;
	bottom_vertex[5] = 1;
	bottom_vertex[6] = 1;

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_components * sizeof GLfloat, nullptr);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertex_components * sizeof GLfloat, reinterpret_cast<void*>(3 * sizeof GLfloat));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	*/
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