#define _USE_MATH_DEFINES

#include <cmath>
#include <glm/gtx/color_space.hpp>

#include "Cube.hpp"
#include "Color.hpp"
#include "Utils.hpp"

//---------------------------------

Cube::Cube():
	m_vertex_buffer(0),
	m_vertex_array(0),
	m_position(0)
{
	constexpr size_t vertex_components = 7;
	GLfloat vertices[] = {
	     0.5, -0.5, -0.5,   1, 1, 1, 1,
		-0.5, -0.5, -0.5,   0, 1, 1, 1,
		-0.5,  0.5, -0.5,   1, 0, 1, 1,
		 0.5, -0.5, -0.5,   1, 1, 0, 1,
		-0.5,  0.5, -0.5,   1, 0, 0, 1,
		 0.5,  0.5, -0.5,   0, 1, 1, 1,

		-0.5,  0.5, -0.5,   0, 1, 1, 1,
		-0.5, -0.5, -0.5,   1, 0, 1, 1,
		-0.5, -0.5,  0.5,   1, 1, 1, 1,
		-0.5,  0.5, -0.5,   0, 1, 0, 1,
		-0.5, -0.5,  0.5,   1, 1, 1, 1,
		-0.5,  0.5,  0.5,   0, 1, 1, 1,

		-0.5, -0.5,  0.5,   1, 1, 1, 1,
		 0.5, -0.5,  0.5,   1, 0, 1, 1,
		 0.5,  0.5,  0.5,   0, 1, 1, 1,
		-0.5, -0.5,  0.5,   1, 1, 1, 1,
		 0.5,  0.5,  0.5,   0, 1, 1, 1,
		-0.5,  0.5,  0.5,   1, 1, 1, 1,

		 0.5, -0.5, -0.5,   1, 1, 1, 1,
		 0.5,  0.5, -0.5,   1, 1, 1, 1,
		 0.5,  0.5,  0.5,   1, 1, 1, 1,
		 0.5, -0.5, -0.5,   1, 1, 1, 1,
		 0.5,  0.5,  0.5,   1, 1, 1, 1,
		 0.5, -0.5,  0.5,   1, 1, 1, 1,

		 0.5,  0.5, -0.5,   1, 1, 1, 1,
		-0.5,  0.5, -0.5,   1, 1, 1, 1,
		-0.5,  0.5,  0.5,   1, 1, 1, 1,
		 0.5,  0.5, -0.5,   1, 1, 1, 1,
		-0.5,  0.5,  0.5,   1, 1, 1, 1,
		 0.5,  0.5,  0.5,   1, 1, 1, 1,

		 0.5, -0.5,  0.5,   1, 1, 1, 1,
		-0.5, -0.5,  0.5,   1, 1, 1, 1,
		-0.5, -0.5, -0.5,   1, 1, 1, 1,
		 0.5, -0.5,  0.5,   1, 1, 1, 1,
		-0.5, -0.5, -0.5,   1, 1, 1, 1,
		 0.5, -0.5, -0.5,   1, 1, 1, 1
	};

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
}

Cube::~Cube()
{
	glDeleteBuffers(1, &m_vertex_buffer);
	glDeleteVertexArrays(1, &m_vertex_array);
}

//---------------------------------

void Cube::draw()
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
	glDrawArrays(GL_TRIANGLES, 0, vertex_count);
	glBindVertexArray(0);
}

//---------------------------------

void Cube::setPosition(const glm::vec3& position)
{
	m_position = position;
}

glm::vec3 Cube::getPosition() const
{
	return m_position;
}

//---------------------------------