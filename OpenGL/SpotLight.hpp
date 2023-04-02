#pragma once

#include "Light.hpp"

//-----------------------------------

class SpotLight: public Light
{
public:
	SpotLight();

	void setPosition(const glm::vec3& position);
	glm::vec3 getPosition() const;

	void setDirection(const glm::vec3& direction);
	glm::vec3 getDirection() const;

	virtual void setDiffuseStrength(float strength);
	virtual float getDiffuseStrength() const;

	virtual void setSpecularStrength(float strength);
	virtual float getSpecularStrength() const;

	void setCutoffAngle(float angle);
	float getCutoffAngle() const;
	
	void setOuterCutoffAngle(float angle);
	float getOuterCutoffAngle() const;

	void resetCounter(Shader& shader) const;
	void apply(Shader& shader) const;

protected:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	float m_diffuse_strength;
	float m_specular_strength;
	float m_cutoff_angle;
	float m_outer_cutoff_angle;

private:
	static int s_counter;

};

//-----------------------------------