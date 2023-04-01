#define _USE_MATH_DEFINES
#include <cmath>
#include "Sphere.hpp"

//---------------------------------

glm::vec3 CalculateSpherePoint(float radius, double alpha, double beta);

//---------------------------------

Sphere::Sphere():
	Primitive(),
	m_point_count(16),
	m_radius(.5f)
{
	updateVertexData();
}

//---------------------------------

void Sphere::setRadius(float radius)
{
	if (radius != m_radius)
		m_radius = radius, updateVertexData();
}

float Sphere::getRadius() const
{
	return m_radius;
}

void Sphere::setPointCount(const glm::uvec2& count)
{
	if (m_point_count != count)
	{
		m_point_count = count;
		updateVertexData();
	}
}

glm::uvec2 Sphere::getPointCount() const
{
	return m_point_count;
}

//---------------------------------

void Sphere::calculateVertices()
{
	for (unsigned y = 0; y < m_point_count.y; y++)
	{
		double ty_current   = static_cast<double>(y  )/m_point_count.y;
		double ty_next      = static_cast<double>(y+1)/m_point_count.y;
		double beta_current = ty_current * M_PI*2;
		double beta_next    = ty_next    * M_PI*2;

		for (unsigned x = 0; x < m_point_count.x; x++)
		{
			double tx_current    = static_cast<double>(x  )/m_point_count.x;
			double tx_next       = static_cast<double>(x+1)/m_point_count.x;
			double alpha_current = M_PI/2 - tx_current * M_PI;
			double alpha_next    = M_PI/2 - tx_next    * M_PI;

			Vertex a(CalculateSpherePoint(m_radius, alpha_current, beta_current), glm::vec2(tx_current, ty_current));
			Vertex b(CalculateSpherePoint(m_radius, alpha_current, beta_next   ), glm::vec2(tx_current, ty_next   ));
			Vertex c(CalculateSpherePoint(m_radius, alpha_next,    beta_current), glm::vec2(tx_next,    ty_current));
			Vertex d(CalculateSpherePoint(m_radius, alpha_next,    beta_next   ), glm::vec2(tx_next,    ty_next   ));

			m_vertex_buffer += c;
			m_vertex_buffer += b;
			m_vertex_buffer += a;

			m_vertex_buffer += b;
			m_vertex_buffer += c;
			m_vertex_buffer += d;
		}
	}

	m_vertex_buffer.commit();
}

//---------------------------------

glm::vec3 CalculateSpherePoint(float radius, double alpha, double beta)
{
	return radius*glm::vec3(
		cos(alpha)*cos(beta),
		cos(alpha)*sin(beta),
		sin(alpha)
	);
}

//---------------------------------