#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

//---------------------------------

#pragma pack(push, 1)
class Vertex
{
public:
	Vertex();
	Vertex(const Vertex& copy);
	Vertex(GLfloat x, GLfloat y, GLfloat z);
	Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.f);
	Vertex(const glm::vec3& position);
	Vertex(const glm::vec3& position, const glm::vec4& color);

	// Returns component count of a single vertex
	static constexpr size_t Components() {
		return sizeof Vertex / sizeof GLfloat;
	}

	GLfloat x;
	GLfloat y;
	GLfloat z;

	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
};
#pragma pack(pop)

//---------------------------------