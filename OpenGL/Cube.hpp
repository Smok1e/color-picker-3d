#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include "Shader.hpp"

//---------------------------------

class Cube
{
public:
	Cube();
	~Cube();

	void draw();

	void      setPosition(const glm::vec3& position);
	glm::vec3 getPosition() const;

protected:
	GLuint m_vertex_buffer;
	GLuint m_vertex_array;
	glm::vec3 m_position;

};

//---------------------------------