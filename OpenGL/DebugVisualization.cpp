#include "DebugVisualization.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"

//-----------------------------------

void DrawVector(Shader* shader, glm::vec3 position, glm::vec3 vector, Color color /*= Color::Red*/)
{
	constexpr const float length    = .3f;
	constexpr const float thickness = .002f;

	static Cylinder cylinder;
	static Cone     cone;

	static bool initialized = false;
	if (!initialized)
	{
		cylinder.setLightningEnabled(false);
		cylinder.setRadius(thickness);
		cylinder.setHeight(length);

		cone.setLightningEnabled(false);
		cone.setRadius(thickness*4);
		cone.setHeight(thickness*8);

		initialized = true;
	}

	glm::vec3 direction = glm::normalize(vector);
	cylinder.setColor(color);
	cylinder.setDirection(direction);
	cylinder.setPosition(position+direction*length*.5f);

	cone.setColor(color);
	cone.setDirection(direction);
	cone.setPosition(position+direction*length);

	cylinder.draw(shader);
	cone.draw(shader);
}

//-----------------------------------