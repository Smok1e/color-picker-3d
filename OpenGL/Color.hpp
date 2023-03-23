#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <GL/glew.h>

//-----------------------------------

#pragma pack(push, 1)
class Color
{
public:
	template<size_t Components>
	using glm_vec_t = glm::vec<Components, GLfloat, glm::packed_highp>;
	typedef Color::glm_vec_t<4> glm_vec4_t;
	typedef Color::glm_vec_t<3> glm_vec3_t;

	Color();
	Color(const Color& copy);
	Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.f);
	Color(const glm_vec4_t& glm_vector);
	Color(const glm_vec3_t& glm_vector);

	static Color FromBytesRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	static Color FromStringHexRGB(const char* hexstring);
	static Color Random();

	char* toStringHexRGB(char* buffer, size_t limit = 0) const;

	constexpr operator glm_vec4_t() const { return glm_vector; }
	constexpr operator glm_vec3_t() const { return glm_vector; }

	constexpr bool operator==(const Color& that) const {
		return r == that.r && g == that.g && b == that.b && a == that.a;
	}

	constexpr bool operator!=(const Color& that) const {
		return r != that.r || g != that.g || b != that.b || a != that.a;
	}

	union
	{
		struct { GLfloat r, g, b, a; };
		GLfloat data[4];
		glm_vec4_t glm_vector;
	};

	static const Color Transparent;
	static const Color White;
	static const Color Black;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
};
#pragma pack(pop)

//-----------------------------------