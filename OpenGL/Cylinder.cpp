#define _USE_MATH_DEFINES
#include <cmath>

#include "Cylinder.hpp"

//---------------------------------

Cylinder::Cylinder():
	Primitive(),
	m_radius(1),
	m_height(1),
	m_point_count(16)
{
	updateVertexData();
}

//---------------------------------

void Cylinder::setRadius(float radius)
{
	if (m_radius != radius)
		m_radius = radius, updateVertexData();
}

float Cylinder::getRadius() const
{
	return m_radius;
}

void Cylinder::setHeight(float height)
{
	if (m_height != height)
		m_height = height, updateVertexData();
}

float Cylinder::getHeight() const
{
	return m_height;
}

void Cylinder::setPointCount(unsigned point_count)
{
	if (m_point_count != point_count)
		m_point_count = point_count, updateVertexData();
}

unsigned Cylinder::getPointCount() const
{
	return m_point_count;
}

//---------------------------------

void Cylinder::calculateVertices()
{
	for (unsigned point = 0; point < m_point_count; point++)
	{
		double t_current = static_cast<double>(point)  /m_point_count;
		double t_next    = static_cast<double>(point+1)/m_point_count;

		double alpha = M_PI*2*t_current;
		double beta  = M_PI*2*t_next;

		//    .-----.
		//   /       \  <-- cylinder
		//  |\       /|
		//  | a-----b-------------------+
		//  | |   / | |	                |
		//  ! |  /  | !	   texture      |
		//   \| /   |/	                |
		//    c-----d-------------------+

		glm::vec3 a(m_radius*cos(alpha), 0.5*m_height, m_radius*sin(alpha));
		glm::vec3 b(m_radius*cos(beta ), 0.5*m_height, m_radius*sin(beta ));
		glm::vec3 c = a - glm::vec3(0, m_height, 0);
		glm::vec3 d = b - glm::vec3(0, m_height, 0);

		m_vertex_buffer += Vertex(a, glm::vec2(t_current, 0));
		m_vertex_buffer += Vertex(b, glm::vec2(t_next,    0));
		m_vertex_buffer += Vertex(c, glm::vec2(t_current, 1));

		m_vertex_buffer += Vertex(b, glm::vec2(t_next,    0));
		m_vertex_buffer += Vertex(d, glm::vec2(t_next,    1));
		m_vertex_buffer += Vertex(c, glm::vec2(t_current, 1));
	}

	m_vertex_buffer.commit();
}

//---------------------------------