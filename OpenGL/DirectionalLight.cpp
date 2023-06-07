#define _CRT_SECURE_NO_WARNINGS

#include "DirectionalLight.hpp"

//---------------------------------

DirectionalLight::DirectionalLight():
	Light(),
	m_direction(glm::vec3(0, 0, 0))
{}

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
	static char instance[BUFFSIZE] = "";
	char* field = instance + sprintf_s(instance, "dirLights[%d].", Light::s_directional_light_count);

	#define setuniform(name, value) strcpy(field, name); shader.setUniform(instance, value);
	setuniform("direction",          m_direction        );
	setuniform("color",              m_color            );
	setuniform("ambient_strength",   m_ambient_strength );
	setuniform("diffuse_strength",   m_diffuse_strength );
	setuniform("specular_strength",  m_specular_strength);
	#undef setuniform

	Light::s_directional_light_count++;
}

//---------------------------------