#pragma once

#include "Light.hpp"

//---------------------------------

class DirectionalLight: public Light
{
public:
	DirectionalLight();

	virtual void setDirection(const glm::vec3& direction);
	virtual glm::vec3 getDirection() const;

	virtual void setDiffuseStrength(float strength);
	virtual float getDiffuseStrength() const;

	virtual void setSpecularStrength(float strength);
	virtual float getSpecularStrength() const;

	virtual void resetCounter(Shader& shader) const;
	virtual void apply(Shader& shader) const;

protected:
	glm::vec3 m_direction;
	float m_diffuse_strength;
	float m_specular_strength;

private:
	static int s_counter;

};

//---------------------------------