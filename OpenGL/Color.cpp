#include "Color.hpp"
#include <cstring>

//-----------------------------------

#pragma warning(push)
#pragma warning(disable: 26495)

Color::Color():
	r(0.f),
	g(0.f),
	b(0.f),
	a(1.f)
{}

Color::Color(const Color& copy):
	r(copy.r),
	g(copy.g),
	b(copy.b),
	a(copy.a)
{}

Color::Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a /*= 1.f*/):
	r(r),
	g(g),
	b(b),
	a(a)
{}

Color::Color(const Color::glm_vec4_t& glm_vector):
	glm_vector(glm_vector)
{}

Color::Color(const Color::glm_vec3_t& glm_vector):
	r(glm_vector.r),
	g(glm_vector.g),
	b(glm_vector.b),
	a(1.f)
{}

Color::Color(const char* hexstring):
	r(0),
	g(0),
	b(0),
	a(0)
{
	*this = FromStringHexRGB(hexstring);
}

#pragma warning(pop)

//-----------------------------------

Color Color::FromBytesRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a /*= 255*/)
{
	return Color(
		static_cast<GLfloat>(r)/255.f,
		static_cast<GLfloat>(g)/255.f,
		static_cast<GLfloat>(b)/255.f,
		static_cast<GLfloat>(a)/255.f
	);
}

Color Color::FromStringHexRGB(const char* hexstring)
{
	hexstring += (*hexstring == '#');
	size_t len = strlen(hexstring);
	if (len != 6 && len != 8)
		return Color::Black;

	uint8_t bytes[4] = {0};
	*reinterpret_cast<uint32_t*>(bytes) = strtoull(hexstring, nullptr, 16);

	if (len == 6) return Color::FromBytesRGB(bytes[2], bytes[1], bytes[0]          );
	else          return Color::FromBytesRGB(bytes[3], bytes[2], bytes[1], bytes[0]);
}

Color Color::Random()
{
	return Color::FromBytesRGB(rand()%256, rand()%256, rand()%256);
}

//-----------------------------------

char* Color::toStringHexRGB(char* buffer, size_t limit /*= 0*/) const
{
	uint8_t bytes[4] = {
		255*r,
		255*g,
		255*b,
		255*a,
	};

	snprintf(buffer, limit, "%02X%02X%02X%02X", bytes[0], bytes[1], bytes[2], bytes[3]);
	return buffer;
}	

//----------------------------------- Constants

const Color Color::Transparent(0.f, 0.f, 0.f, 0.f);
const Color Color::Black      (0.f, 0.f, 0.f     );
const Color Color::White      (1.f, 1.f, 1.f     );
const Color Color::Red        (1.f, 0.f, 0.f     );
const Color Color::Green      (0.f, 1.f, 0.f     );
const Color Color::Blue       (0.f, 0.f, 1.f     );

//-----------------------------------

#if 0
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
#endif

//-----------------------------------