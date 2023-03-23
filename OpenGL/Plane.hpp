#pragma once

#include "Primitive.hpp"

//---------------------------------

class Plane: public Primitive
{
public:
	Plane();

	virtual void draw(Shader* shader = nullptr) const;

	virtual void      setSize(glm::vec2 size);
	virtual glm::vec2 getSize() const;

protected:
	virtual void updateVertexData();

	glm::vec2 m_size;

};

//---------------------------------