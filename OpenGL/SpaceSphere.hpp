#pragma once

//-----------------------------------

#include "Sphere.hpp"

//-----------------------------------

class SpaceSphere: public Sphere
{
public:
	SpaceSphere();

protected:
	virtual void bindShader(Shader& shader) const;

};

//-----------------------------------