#include "Light.hpp"

//---------------------------------

Light::Light():
	m_color()
{}

//---------------------------------

void Light::setColor(const Color& color)
{
	m_color = color;
}

Color Light::getColor() const
{
	return m_color;
}

//---------------------------------