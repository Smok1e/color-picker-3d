#pragma once

#include "Primitive.hpp"

//---------------------------------

class Cube: public Primitive
{
public:
	Cube();

	virtual void setSize(const glm::vec3& size);
	virtual glm::vec3 getSize() const;

protected:
	virtual void calculateVertices();

	glm::vec3 m_size;

};

//---------------------------------