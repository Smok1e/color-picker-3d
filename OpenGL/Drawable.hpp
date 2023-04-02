#pragma once

#include "Shader.hpp"

//---------------------------------

class Drawable
{
public:
	Drawable();
	
	virtual void draw(Shader& shader) const = 0;
	virtual void afterDraw(Shader& shader) const;
	virtual void drawDebugGui();

};

//---------------------------------