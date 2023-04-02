#pragma once

#include "Color.hpp"
#include "Shader.hpp"

//---------------------------------

class Light
{
public:
	static const unsigned LightsLimit = 16;

	Light();

	virtual void setColor(const Color& color);
	virtual Color getColor() const;

	virtual void resetCounter(Shader& shader) const = 0;
	virtual void apply(Shader& shader) const = 0;

protected:
	Color m_color;

};

//---------------------------------