#pragma once

#include "Primitive.hpp"

//---------------------------------

class Cube: public Primitive
{
public:
	Cube();

	virtual void draw(Shader* shader = nullptr) const;

	virtual void setSize(const glm::vec3& size);
	virtual glm::vec3 getSize() const;

protected:
	virtual void updateVertexData();

	glm::vec3 m_size;

};

//---------------------------------