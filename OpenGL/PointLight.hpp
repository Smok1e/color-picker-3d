#pragma once

#include "Light.hpp"

//---------------------------------

class PointLight: public Light
{
public:
	PointLight();

	virtual void setPosition(const glm::vec3& position);
	virtual glm::vec3 getPosition() const;

	virtual void setDiffuseStrength(float strength);
	virtual float getDiffuseStrength() const;

	virtual void setSpecularStrength(float strength);
	virtual float getSpecularStrength() const;

	virtual void resetCounter(Shader& shader) const;
	virtual void apply(Shader& shader) const;

protected:
	glm::vec3 m_position;
	float m_diffuse_strength;
	float m_specular_strength;

private:
	static int s_counter;

};

//---------------------------------