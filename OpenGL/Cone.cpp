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

void Cone::draw(Shader* shader /*= nullptr*/) const
{
	Primitive::draw(shader);
	glBindVertexArray(m_vertex_array);
	glDrawArrays(GL_TRIANGLES, 0, m_vertex_buffer.getVertexCount());
	glBindVertexArray(0);
}

//-----------------------------------

void Cone::updateVertexData()
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

		glm::vec3 a(cos(alpha)*m_radius, -0.5*m_height, sin(alpha)*m_radius);
		glm::vec3 b(cos(beta )*m_radius, -0.5*m_height, sin(beta )*m_radius);

		glm::vec3 normal = glm::normalize(glm::cross(b-top, a-top));

		m_vertex_buffer += Vertex(a,   glm::vec2(t_current,                        1), normal);
		m_vertex_buffer += Vertex(b,   glm::vec2(t_next,                           1), normal);
		m_vertex_buffer += Vertex(top, glm::vec2(t_current+(t_next-t_current)*0.5, 0), normal);
	}

	m_vertex_buffer.commit();
	m_vertex_buffer.bind();

	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	Vertex::InitAttributes();
	VertexBuffer::Unbind();
	glBindVertexArray(0);
}

//-----------------------------------