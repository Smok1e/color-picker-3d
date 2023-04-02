#pragma once

#include "Drawable.hpp"

//---------------------------------

class Light: public Drawable
{
public:
	static const unsigned LightsLimit = 16;

	Light();

	virtual void setColor(const Color& color);
	virtual Color getColor() const;

	virtual void setAmbientStrength(float strength);
	virtual float getAmbientStrength() const;

	virtual void setDiffuseStrength(float strength);
	virtual float getDiffuseStrength() const;

	virtual void setSpecularStrength(float strength);
	virtual float getSpecularStrength() const;

	virtual void draw(Shader& shader) const = 0;
	virtual void afterDraw(Shader& shader) const = 0;

	virtual void drawDebugGui();

protected:
	Color m_color;
	float m_ambient_strength;
	float m_diffuse_strength;
	float m_specular_strength;

};

//---------------------------------