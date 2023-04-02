#include "AmbientLight.hpp"

//---------------------------------

AmbientLight::AmbientLight():
	Light(),
	m_strength(.2f)
{}

int AmbientLight::s_counter = 0;

//---------------------------------

void AmbientLight::setStrength(float strength)
{
	m_strength = strength;
}

float AmbientLight::getStrength() const
{
	return m_strength;
}

//---------------------------------

void AmbientLight::resetCounter(Shader& shader) const
{
	if (s_counter != -1)
	{
		shader["ambientLightCount"] = s_counter;
		s_counter = -1;
	}
}

void AmbientLight::apply(Shader& shader) const
{
	s_counter += (s_counter == -1);
	shader.setUniformFormatted(m_color,    "ambientLights[%zu].color",    s_counter);
	shader.setUniformFormatted(m_strength, "ambientLights[%zu].strength", s_counter);
	s_counter++;
}

//---------------------------------