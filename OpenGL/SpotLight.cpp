#define _USE_MATH_DEFINES

#include <cmath>
#include <imgui.h>
#include <ImGuizmo.h>

#include "SpotLight.hpp"

//-----------------------------------

SpotLight::SpotLight():
	Light(),
	m_position(glm::vec3(0, 0, 0)),
	m_direction(glm::vec3(0, 0, 0)),
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
	s_counter += (s_counter == -1);		   
	shader.setUniformFormatted(m_position,                "spotLights[%zu].position",           s_counter);
	shader.setUniformFormatted(m_direction,               "spotLights[%zu].direction",          s_counter);
	shader.setUniformFormatted(m_color,                   "spotLights[%zu].color",              s_counter);
	shader.setUniformFormatted(m_ambient_strength,        "spotLights[%zu].ambient_strength",   s_counter);
	shader.setUniformFormatted(m_diffuse_strength,        "spotLights[%zu].diffuse_strength",   s_counter);
	shader.setUniformFormatted(m_specular_strength,       "spotLights[%zu].specular_strength",  s_counter);
	shader.setUniformFormatted(cos(m_cutoff_angle),       "spotLights[%zu].cutoff_angle",       s_counter);
	shader.setUniformFormatted(cos(m_outer_cutoff_angle), "spotLights[%zu].outer_cutoff_angle", s_counter);
	s_counter++;
}

void SpotLight::afterDraw(Shader& shader) const
{
	if (s_counter != -1)
	{
		shader["spotLightCount"] = s_counter;
		s_counter = -1;
	}
}

//-----------------------------------

void SpotLight::drawDebugGui()
{
	Light::drawDebugGui();

	//ImGui::gizmo

	ImGui::SliderFloat("Inner cutoff angle", &m_cutoff_angle,       0, M_PI*2, "%.2f");
	ImGui::SliderFloat("Outer cutoff angle", &m_outer_cutoff_angle, 0, M_PI*2, "%.2f");
}

//-----------------------------------