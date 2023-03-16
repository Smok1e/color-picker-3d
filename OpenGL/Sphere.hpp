#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "Color.hpp"

//---------------------------------

class Sphere
{
public:
	Sphere();
	~Sphere();

	void destroy();
	void draw(Shader* shader = nullptr);

	void      setPosition(const glm::vec3& position);
	glm::vec3 getPosition() const;

	void setPointCount(size_t latitude, size_t longitude);
	size_t getLatitudePointCount() const;
	size_t getLongitudePointCount() const;

	void setColor(const Color& color);
	Color getColor() const;

	void setRadius(float radius);
	float getRadius() const;

protected:
	void updateVertexData();

	GLuint m_vertex_buffer;
	GLuint m_vertex_array;
	GLuint m_index_buffer;
	size_t m_index_count;
	glm::vec3 m_position;

	size_t m_latitude_points;
	size_t m_longitude_points;
	float m_radius;
	Color m_color;

};

//---------------------------------