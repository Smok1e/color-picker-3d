#include "PointLight.hpp"

//---------------------------------

PointLight::PointLight():
	Light(),
	m_position(glm::vec3(0, 0, 0)),
	m_diffuse_strength (0.8f),
	m_specular_strength(0.5f)
{}

int PointLight::s_counter = 0;

//---------------------------------

void PointLight::setPosition(const glm::vec3& position)
{
	m_position = position;
}

glm::vec3 PointLight::getPosition() const
{
	return m_position;
}

void PointLight::setDiffuseStrength(float strength)
{
	m_diffuse_strength = strength;
}

float PointLight::getDiffuseStrength() const
{
	return m_diffuse_strength;
}

void PointLight::setSpecularStrength(float strength)
{
	m_specular_strength = strength;
}

float PointLight::getSpecularStrength() const
{
	return m_specular_strength;
}

//---------------------------------

void PointLight::resetCounter(Shader& shader) const
{
	if (s_counter != -1)
	{
		shader["pointLightCount"] = s_counter;
		s_counter = -1;
	}
}

void PointLight::apply(Shader& shader) const
{
	s_counter += (s_counter == -1);
	shader.setUniformFormatted(m_position,          "pointLights[%zu].position",          s_counter);
	shader.setUniformFormatted(m_color,             "pointLights[%zu].color",             s_counter);
	shader.setUniformFormatted(m_diffuse_strength,  "pointLights[%zu].diffuse_strength",  s_counter);
	shader.setUniformFormatted(m_specular_strength, "pointLights[%zu].specular_strength", s_counter);
	s_counter++;
}

//---------------------------------