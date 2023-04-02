#include "DirectionalLight.hpp"

//---------------------------------

DirectionalLight::DirectionalLight():
	Light(),
	m_direction(glm::vec3(0, 0, 0))
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

//---------------------------------

void DirectionalLight::draw(Shader& shader) const
{
	s_counter += (s_counter == -1);
	shader.setUniformFormatted(m_direction,         "dirLights[%zu].direction",         s_counter);
	shader.setUniformFormatted(m_color,             "dirLights[%zu].color",             s_counter);
	shader.setUniformFormatted(m_ambient_strength,  "dirLights[%zu].ambient_strength",  s_counter);
	shader.setUniformFormatted(m_diffuse_strength,  "dirLights[%zu].diffuse_strength",  s_counter);
	shader.setUniformFormatted(m_specular_strength, "dirLights[%zu].specular_strength", s_counter);
	s_counter++;
}

void DirectionalLight::afterDraw(Shader& shader) const
{
	if (s_counter != -1)
	{
		shader["dirLightCount"] = s_counter;
		s_counter = -1;
	}
}

//---------------------------------