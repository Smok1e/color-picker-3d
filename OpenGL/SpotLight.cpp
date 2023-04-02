#include "SpotLight.hpp"

//-----------------------------------

SpotLight::SpotLight():
	Light(),
	m_position(glm::vec3(0, 0, 0)),
	m_direction(glm::vec3(0, 0, 0)),
	m_diffuse_strength (0.8f),
	m_specular_strength(0.5f),
	m_cutoff_angle(0),
	m_outer_cutoff_angle(0)
{}

int SpotLight::s_counter = 0;

//-----------------------------------

void SpotLight::setPosition(const glm::vec3& position)
{
	m_position = position;
}

glm::vec3 SpotLight::getPosition() const
{
	return m_position;
}

void SpotLight::setDirection(const glm::vec3& direction)
{
	m_direction = direction;
}

glm::vec3 SpotLight::getDirection() const
{
	return m_direction;
}

void SpotLight::setDiffuseStrength(float strength)
{
	m_diffuse_strength = strength;
}

float SpotLight::getDiffuseStrength() const
{
	return m_diffuse_strength;
}

void SpotLight::setSpecularStrength(float strength)
{
	m_specular_strength = strength;
}

float SpotLight::getSpecularStrength() const
{
	return m_specular_strength;
}

void SpotLight::setCutoffAngle(float angle)
{
	m_cutoff_angle = angle;
}

float SpotLight::getCutoffAngle() const
{
	return m_cutoff_angle;
}

void SpotLight::setOuterCutoffAngle(float angle)
{
	m_outer_cutoff_angle = angle;
}

float SpotLight::getOuterCutoffAngle() const
{
	return m_outer_cutoff_angle;
}

//-----------------------------------

void SpotLight::resetCounter(Shader& shader) const
{
	if (s_counter != -1)
	{
		shader["spotLightCount"] = s_counter;
		s_counter = -1;
	}
}

void SpotLight::apply(Shader& shader) const
{
	s_counter += (s_counter == -1);		   
	shader.setUniformFormatted(m_position,           "spotLights[%zu].position",           s_counter);
	shader.setUniformFormatted(m_direction,          "spotLights[%zu].direction",          s_counter);
	shader.setUniformFormatted(m_color,              "spotLights[%zu].color",              s_counter);
	shader.setUniformFormatted(m_diffuse_strength,   "spotLights[%zu].diffuse_strength",   s_counter);
	shader.setUniformFormatted(m_specular_strength,  "spotLights[%zu].specular_strength",  s_counter);
	shader.setUniformFormatted(m_cutoff_angle,       "spotLights[%zu].cutoff_angle",       s_counter);
	shader.setUniformFormatted(m_outer_cutoff_angle, "spotLights[%zu].outer_cutoff_angle", s_counter);
	s_counter++;
}

//-----------------------------------