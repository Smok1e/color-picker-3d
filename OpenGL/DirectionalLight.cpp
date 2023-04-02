#include "DirectionalLight.hpp"

//---------------------------------

DirectionalLight::DirectionalLight():
	Light(),
	m_direction(glm::vec3(0, 0, 0)),
	m_diffuse_strength (0.8f),
	m_specular_strength(0.5f)
{}

int DirectionalLight::s_counter = 0;

//---------------------------------

void DirectionalLight::setDirection(const glm::vec3& direction)
{
	m_direction = direction;
}

glm::vec3 DirectionalLight::getDirection() const
{
	return m_direction;
}

void DirectionalLight::setDiffuseStrength(float strength)
{
	m_diffuse_strength = strength;
}

float DirectionalLight::getDiffuseStrength() const
{
	return m_diffuse_strength;
}

void DirectionalLight::setSpecularStrength(float strength)
{
	m_specular_strength = strength;
}

float DirectionalLight::getSpecularStrength() const
{
	return m_specular_strength;
}

//---------------------------------

void DirectionalLight::resetCounter(Shader& shader) const
{
	if (s_counter != -1)
	{
		shader["dirLightCount"] = s_counter;
		s_counter = -1;
	}
}

void DirectionalLight::apply(Shader& shader) const
{
	s_counter += (s_counter == -1);
	shader.setUniformFormatted(m_direction,         "dirLights[%zu].direction",          s_counter);
	shader.setUniformFormatted(m_color,             "dirLights[%zu].color",              s_counter);
	shader.setUniformFormatted(m_diffuse_strength,  "dirLights[%zu].diffuse_strength",   s_counter);
	shader.setUniformFormatted(m_specular_strength, "dirLights[%zu].specular_strength",  s_counter);
	s_counter++;
}

//---------------------------------