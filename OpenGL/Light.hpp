#pragma once

#include "Drawable.hpp"

//---------------------------------

class Light: public Drawable
{
public:
	static const unsigned LightsLimit = 16;

	Light();

	static void Update(Shader& shader);

	virtual void setColor(const Color& color);
	virtual Color getColor() const;

	virtual void setAmbientStrength(float strength);
	virtual float getAmbientStrength() const;

	virtual void setDiffuseStrength(float strength);
	virtual float getDiffuseStrength() const;

	virtual void setSpecularStrength(float strength);
	virtual float getSpecularStrength() const;

	virtual void draw(Shader& shader) const = 0;

	virtual void drawDebugGui();

protected:
	Color m_color;
	float m_ambient_strength;
	float m_diffuse_strength;
	float m_specular_strength;

	static int s_point_light_count;
	static int s_directional_light_count;
	static int s_spot_light_count;

};

//---------------------------------