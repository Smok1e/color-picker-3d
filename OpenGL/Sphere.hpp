#pragma once

#include "Primitive.hpp"

//---------------------------------

class Sphere: public Primitive
{
public:
	Sphere();

	virtual void setRadius(float radius);
	virtual float getRadius() const;

	virtual void setPointCount(const glm::uvec2& count);
	virtual glm::uvec2 getPointCount() const;

protected:
	virtual void calculateVertices();

	glm::uvec2 m_point_count;
	float m_radius;

};

//---------------------------------