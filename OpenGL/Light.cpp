#include "Light.hpp"

//---------------------------------

Light::Light():
	m_position(),
	m_color(),
	m_ambient_strength (0.2),
	m_diffuse_strength (0.8),
	m_specular_strength(0.5)
{}

//---------------------------------

void Light::setPosition(const glm::vec3& position)
{
	m_position = position;
}

glm::vec3 Light::getPosition() const
{
	return m_position;
}

void Light::setColor(const Color& color)
{
	m_color = color;
}

Color Light::getColor() const
{
	return m_color;
}

void Light::setAmbientStrength(float strength)
{
	m_ambient_strength = strength;
}

float Light::getAmbientStrength() const
{
	return m_ambient_strength;
}

void Light::setDiffuseStrength(float strength)
{
	m_diffuse_strength = strength;
}

float Light::getDiffuseStrength() const
{
	return m_diffuse_strength;
}

void Light::setSpecularStrength(float strength)
{
	m_specular_strength = strength;
}

float Light::getSpecularStrength() const
{
	return m_specular_strength;
}

//---------------------------------