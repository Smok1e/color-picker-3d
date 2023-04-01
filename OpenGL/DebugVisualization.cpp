#include "DebugVisualization.hpp"
#include "Material.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"

//-----------------------------------

void DrawVector(Shader* shader, glm::vec3 position, glm::vec3 vector, Color color /*= Color::Red*/)
{
	constexpr const float length    = .3f;
	constexpr const float thickness = .002f;

	static Material material;
	static Cylinder cylinder;
	static Cone     cone;

	static bool initialized = false;
	if (!initialized)
	{
		material.setLightningEnabled(false);
		cylinder.setMaterial(&material);
		cone.setMaterial(&material);

		cylinder.setRadius(thickness);
		cylinder.setHeight(length);

		cone.setRadius(thickness*4);
		cone.setHeight(thickness*8);

		initialized = true;
	}

	material.setColor(color);
	glm::vec3 direction = glm::normalize(vector);

	cylinder.setRotation(direction);
	cylinder.setPosition(position+direction*length*.5f);

	cone.setRotation(direction);
	cone.setPosition(position+direction*length);

	cylinder.draw(shader);
	cone.draw(shader);
}

//-----------------------------------