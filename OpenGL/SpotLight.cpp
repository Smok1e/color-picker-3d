#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <cmath>
#include <imgui.h>
// #include <ImGuizmo.h>

#include "SpotLight.hpp"

//-----------------------------------

SpotLight::SpotLight():
	Light(),
	m_position(glm::vec3(0, 0, 0)),
	m_direction(glm::vec3(0, 0, 0)),
	m_cutoff_angle(0),
	m_outer_cutoff_angle(0)
{}

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

void SpotLight::draw(Shader& shader) const
{
	static char instance[BUFFSIZE] = "";
	char* field = instance + sprintf_s(instance, "spotLights[%d].", Light::s_spot_light_count);

	#define setuniform(name, value) strcpy(field, name); shader.setUniform(instance, value);
	setuniform("position",           m_position               );
	setuniform("direction",          m_direction              );
	setuniform("color",              m_color                  );
	setuniform("ambient_strength",   m_ambient_strength       );
	setuniform("diffuse_strength",   m_diffuse_strength       );
	setuniform("specular_strength",  m_specular_strength      );
	setuniform("cutoff_angle",       cos(m_cutoff_angle)      );
	setuniform("outer_cutoff_angle", cos(m_outer_cutoff_angle));
	#undef setuniform

	Light::s_spot_light_count++;
}

//-----------------------------------

void SpotLight::drawDebugGui()
{
	Light::drawDebugGui();
	ImGui::SliderFloat("Inner cutoff angle", &m_cutoff_angle,       0, M_PI*2, "%.2f");
	ImGui::SliderFloat("Outer cutoff angle", &m_outer_cutoff_angle, 0, M_PI*2, "%.2f");
}

//-----------------------------------