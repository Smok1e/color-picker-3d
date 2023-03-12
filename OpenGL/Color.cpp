#include "Color.hpp"

//-----------------------------------

Color::Color():
	r(0),
	g(0),
	b(0),
	a(255)
{}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a /*= 255*/):
	r(r),
	g(g),
	b(b),
	a(a)
{}

Color::Color(const Color& copy):
	r(copy.r),
	g(copy.g),
	b(copy.b),
	a(copy.a)
{}

//-----------------------------------

glm::vec3 Color::asVec3()
{
	return glm::vec3(
		static_cast<float>(r) / 255,
		static_cast<float>(g) / 255,
		static_cast<float>(b) / 255
	);
}

glm::vec4 Color::asVec4()
{
	return glm::vec4(
		static_cast<float>(r) / 255,
		static_cast<float>(g) / 255,
		static_cast<float>(b) / 255,
		static_cast<float>(a) / 255
	);
}

Color::operator glm::vec3()
{
	return asVec3();
}

Color::operator glm::vec4()
{
	return asVec4();
}

//-----------------------------------

Color Color::HSV(uint8_t h, uint8_t s, uint8_t v, uint8_t a /*= 255*/)
{
	Color color;

	if (!s)
	{
		color.r = v;
		color.g = v;
		color.b = v;
		return color;
	}

	int region = h / 43;
	int remainder = (h - (region * 43)) * 6;

	int p = (v * (255 - s)) >> 8;
	int q = (v * (255 - ((s * remainder) >> 8))) >> 8;
	int t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

	switch (region)
	{
		case 0:
			color.r = v;
			color.g = t;
			color.b = p;
			break;

		case 1:
			color.r = q;
			color.g = v;
			color.b = p;
			break;

		case 2:
			color.r = p;
			color.g = v;
			color.b = t;
			break;

		case 3:
			color.r = p;
			color.g = q;
			color.b = v;
			break;

		case 4:
			color.r = t;
			color.g = p;
			color.b = v;
			break;

		default:
			color.r = v;
			color.g = p;
			color.b = q;
			break;

	}

	color.a = a;
	return color;
}

//-----------------------------------