#pragma once

#include "Light.hpp"

//---------------------------------

class PointLight: public Light
{
public:
	PointLight();

	virtual void setPosition(const glm::vec3& position);
	virtual glm::vec3 getPosition() const;

	virtual void draw(Shader& shader) const;
	virtual void afterDraw(Shader& shader) const;

protected:
	glm::vec3 m_position;

private:
	static int s_counter;

};

//---------------------------------