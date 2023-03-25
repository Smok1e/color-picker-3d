#pragma once

#include <glm/glm.hpp>
#include "Color.hpp"

//---------------------------------

class Light
{
public:
	Light();

	virtual void setPosition(const glm::vec3& position);
	virtual glm::vec3 getPosition() const;

	virtual void setColor(const Color& color);
	virtual Color getColor() const;

	virtual void setAmbientStrength(float strength);
	virtual float getAmbientStrength() const;

	virtual void setDiffuseStrength(float strength);
	virtual float getDiffuseStrength() const;

	virtual void setSpecularStrength(float strength);
	virtual float getSpecularStrength() const;

protected:
	glm::vec3 m_position;
	Color m_color;

	float m_ambient_strength;
	float m_diffuse_strength;
	float m_specular_strength;

};

//---------------------------------