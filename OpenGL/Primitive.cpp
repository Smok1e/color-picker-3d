#define _USE_MATH_DEFINES

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "Primitive.hpp"
#include "Utils.hpp"

//---------------------------------

Primitive::Primitive():
	m_vertex_buffer(),
	m_vertex_array(0),
	m_position(0),
	m_scale(glm::vec3(1, 1, 1)),
	m_direction(0),
	m_transform(glm::identity<glm::mat4>()),
	m_color(),
	m_texture(nullptr),
	m_normalmap(nullptr),
	m_use_lightning(true)
{
	updateTransform();
}

Primitive::~Primitive()
{
	cleanup();
}

//---------------------------------

void Primitive::draw(Shader* shader /*= nullptr*/) const
{
	if (shader) bindShader(*shader);
}

//---------------------------------

void Primitive::setPosition(const glm::vec3& position)
{
	m_position = position;
}

glm::vec3 Primitive::getPosition() const
{
	return m_position;
}

void Primitive::setScale(const glm::vec3& scale)
{
	m_scale = scale;
}

glm::vec3 Primitive::getScale() const
{
	return m_scale;
}

void Primitive::setDirection(const glm::vec3& direction)
{
	if (m_direction != direction)
		m_direction = direction, updateTransform();
}

glm::vec3 Primitive::getDirection() const
{
	return m_direction;
}

void Primitive::setColor(const Color& color)
{
	m_color = color;
}

Color Primitive::getColor() const
{
	return m_color;
}

void Primitive::setTexture(Texture* texture)
{
	m_texture = texture;
}

Texture* Primitive::getTexture() const
{
	return m_texture;
}

void Primitive::setNormalMap(Texture* normalmap)
{
	m_normalmap = normalmap;
}

Texture* Primitive::getNormalMap() const
{
	return m_normalmap;
}

void Primitive::setLightningEnabled(bool enable)
{
	m_use_lightning = enable;
}

bool Primitive::getLightningEnabled() const
{
	return m_use_lightning;
}

//---------------------------------

void Primitive::updateTransform()
{
	m_transform = glm::rotate(glm::identity<glm::mat4>(), glm::pi<float>(), m_direction+glm::vec3(0, 1, 0));
}

//---------------------------------

void Primitive::cleanup()
{
	if (m_vertex_array) glSafeCallVoid(glDeleteVertexArrays(1, &m_vertex_array));
	m_vertex_array = 0;
}

void Primitive::bindShader(Shader& shader) const
{
	shader["shapeOffset"   ] = m_position;
	shader["shapeColor"    ] = m_color;
	shader["shapeScale"    ] = m_scale;
	shader["shapeTransform"] = m_transform;
	shader["useTexture"    ] = m_texture   != nullptr;
	shader["useNormalMap"  ] = m_normalmap != nullptr;
	shader["useLightning"  ] = m_use_lightning;

	shader.use();
	if (m_texture  ) shader.setUniform("shapeTexture",   *m_texture  );
	if (m_normalmap) shader.setUniform("shapeNormalMap", *m_normalmap);
}

//---------------------------------