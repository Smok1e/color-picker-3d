#pragma once

#include "Light.hpp"

//---------------------------------

class AmbientLight: public Light
{
public:
	AmbientLight();

	virtual void setStrength(float strenth);
	virtual float getStrength() const;

	virtual void resetCounter(Shader& shader) const;
	virtual void apply(Shader& shader) const;

protected:
	float m_strength;

private:
	static int s_counter;

};

//---------------------------------