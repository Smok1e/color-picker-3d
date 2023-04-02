#pragma once

#include "Light.hpp"

//---------------------------------

class DirectionalLight: public Light
{
public:
	DirectionalLight();

	virtual void setDirection(const glm::vec3& direction);
	virtual glm::vec3 getDirection() const;

	virtual void draw(Shader& shader) const;
	virtual void afterDraw(Shader& shader) const;

protected:
	glm::vec3 m_direction;

private:
	static int s_counter;

};

//---------------------------------