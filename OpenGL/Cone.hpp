#pragma once

#include "Primitive.hpp"

//-----------------------------------

class Cone: public Primitive
{
public:
	Cone();

	virtual void setRadius(float radius);
	virtual float getRadius() const;

	virtual void setHeight(float height);
	virtual float getHeight() const;

	virtual void setPointCount(unsigned point_count);
	virtual unsigned getPointCount() const;

protected:
	virtual void calculateVertices();

	float m_radius;
	float m_height;
	unsigned m_point_count;

};

//-----------------------------------