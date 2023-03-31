#pragma once

#include "Primitive.hpp"

//---------------------------------

class Plane: public Primitive
{
public:
	Plane();

	virtual void      setSize(glm::vec2 size);
	virtual glm::vec2 getSize() const;

protected:
	virtual void calculateVertices();

	glm::vec2 m_size;

};

//---------------------------------