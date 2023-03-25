#pragma once

#include "Primitive.hpp"

//---------------------------------

class Sphere: public Primitive
{
public:
	Sphere();

	virtual void draw(Shader* shader = nullptr) const;

	virtual void setRadius(float radius);
	virtual float getRadius() const;

	virtual void setPointCount(size_t latitude, size_t longitude);
	virtual size_t getLatitudePointCount() const;
	virtual size_t getLongitudePointCount() const;

protected:
	virtual void cleanup();
	virtual void updateVertexData();

	GLuint m_index_buffer;
	size_t m_index_count;

	size_t m_latitude_points;
	size_t m_longitude_points;
	float m_radius;

};

//---------------------------------