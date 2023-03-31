#define _USE_MATH_DEFINES
#include <cmath>

#include "Cone.hpp"

//-----------------------------------

Cone::Cone():
	Primitive(),
	m_radius(1),
	m_height(1),
	m_point_count(16)
{
	updateVertexData();
}

//-----------------------------------

void Cone::setRadius(float radius)
{
	if (m_radius != radius)
		m_radius = radius, updateVertexData();
}

float Cone::getRadius() const
{
	return m_radius;
}

void Cone::setHeight(float height)
{
	if (m_height != height)
		m_height = height, updateVertexData();
}

float Cone::getHeight() const
{
	return m_height;
}

void Cone::setPointCount(unsigned point_count)
{
	if (m_point_count != point_count)
		m_point_count = point_count, updateVertexData();
}

unsigned Cone::getPointCount() const
{
	return m_point_count;
}

//-----------------------------------

void Cone::calculateVertices()
{
	//           ^			   
    //           |\			   
    //           | \		   
    //           |  \		   
    // height -> |   \ 		   
    //           |    \		   
    //           |     \	   
    //           +------>	   
	//               ^		   
	//			  radius       

	glm::vec3 top(0, 0.5*m_height, 0);
	for (unsigned point = 0; point < m_point_count; point++)
	{
		double t_current = static_cast<double>(point)  /m_point_count;
		double t_next    = static_cast<double>(point+1)/m_point_count;

		double alpha = M_PI*2*t_current;
		double beta  = M_PI*2*t_next;

		m_vertex_buffer += Vertex(glm::vec3(cos(alpha)*m_radius, -0.5*m_height, sin(alpha)*m_radius), glm::vec2(t_current, 1));
		m_vertex_buffer += Vertex(top, glm::vec2(t_current+(t_next-t_current)*0.5, 0));
		m_vertex_buffer += Vertex(glm::vec3(cos(beta )*m_radius, -0.5*m_height, sin(beta )*m_radius), glm::vec2(t_next,    1));
	}

	m_vertex_buffer.commit();
}

//-----------------------------------