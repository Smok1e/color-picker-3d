#include "Vertex.hpp"

//---------------------------------

Vertex::Vertex():
	x(0),
	y(0),
	z(0),
	r(0),
	g(0),
	b(0),
	a(255)
{}

Vertex::Vertex(const Vertex& copy):
	x(copy.x),
	y(copy.y),
	z(copy.z),
	r(copy.r),
	g(copy.g),
	b(copy.b),
	a(copy.a)
{}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z):
	x(x),
	y(y),
	z(z),
	r(0),
	g(0),
	b(0),
	a(255)
{}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a /*= 1.f*/):
	x(x),
	y(y),
	z(z),
	r(r),
	g(g),
	b(b),
	a(a)
{}

Vertex::Vertex(const glm::vec3& position):
	x(position.x),
	y(position.y),
	z(position.z),
	r(0),
	g(0),
	b(0),
	a(255)
{}

Vertex::Vertex(const glm::vec3& position, const glm::vec4& color):
	x(position.x),
	y(position.y),
	z(position.z),
	r(color.r),
	g(color.g),
	b(color.b),
	a(color.a)
{}

//---------------------------------