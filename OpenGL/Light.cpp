#include <imgui.h>

#include "Light.hpp"

//---------------------------------

Light::Light():
	m_color(),
	m_ambient_strength(0),
	m_diffuse_strength(.4f),
	m_specular_strength(.5f)
{}

int Light::s_point_light_count       = 0;
int Light::s_directional_light_count = 0;
int Light::s_spot_light_count        = 0;

//---------------------------------

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

void Light::drawDebugGui()
{
	ImGui::ColorEdit3("Color", m_color.data);
	ImGui::SliderFloat("Ambient strength", &m_ambient_strength,   0.f, 5.f, "%.2f");
	ImGui::SliderFloat("Diffuse strength", &m_diffuse_strength,   0.f, 5.f, "%.2f");
	ImGui::SliderFloat("Specular strength", &m_specular_strength, 0.f, 5.f, "%.2f");
}

//---------------------------------

void Light::Update(Shader& shader)
{
	shader["pointLightCount"] = s_point_light_count;
	shader["dirLightCount"  ] = s_directional_light_count;
	shader["spotLightCount" ] = s_spot_light_count;

	s_point_light_count = s_directional_light_count = s_spot_light_count = 0;
}

//---------------------------------