#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include "Shader.hpp"

//---------------------------------

class Sphere
{
public:
	Sphere();
	~Sphere();

	void draw();

	void      setPosition(const glm::vec3& position);
	glm::vec3 getPosition() const;

protected:
	GLuint m_vertex_buffer;
	GLuint m_vertex_array;
	glm::vec3 m_position;

};

//---------------------------------