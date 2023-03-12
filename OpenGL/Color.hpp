#pragma once
#include <cstdint>
#include <glm/glm.hpp>

//-----------------------------------

class Color
{
public:
	Color();
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	Color(const Color& copy);

	glm::vec3 asVec3();
	glm::vec4 asVec4();

	operator glm::vec3();
	operator glm::vec4();

	static Color HSV(uint8_t h, uint8_t s, uint8_t v, uint8_t a = 255);

	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

};

//-----------------------------------