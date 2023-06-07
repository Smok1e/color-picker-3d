#define _CRT_SECURE_NO_WARNINGS

#include "PointLight.hpp"

//---------------------------------

PointLight::PointLight():
	Light(),
	m_position(glm::vec3(0, 0, 0))
{}

//---------------------------------

void PointLight::setPosition(const glm::vec3& position)
{
	m_position = position;
}

glm::vec3 PointLight::getPosition() const
{
	return m_position;
}

//---------------------------------
 
void PointLight::draw(Shader& shader) const
{
	static char instance[BUFFSIZE] = "";
	char* field = instance + sprintf_s(instance, "pointLights[%d].", Light::s_point_light_count);

	#define setuniform(name, value) strcpy(field, name); shader.setUniform(instance, value);
	setuniform("position",          m_position         );
	setuniform("color",             m_color            );
	setuniform("ambient_strength",  m_ambient_strength );
	setuniform("diffuse_strength",  m_diffuse_strength );
	setuniform("specular_strength", m_specular_strength);
	#undef setuniform

	Light::s_point_light_count++;
}

//---------------------------------