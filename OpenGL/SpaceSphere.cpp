#include "SpaceSphere.hpp"

//-----------------------------------

SpaceSphere::SpaceSphere():
	Sphere()
{}

//-----------------------------------

void SpaceSphere::bindShader(Shader& shader) const
{
	Sphere::bindShader(shader);
	shader["useBloom"] = true;
}

//-----------------------------------