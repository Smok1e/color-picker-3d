#pragma once

#include "VertexBuffer.hpp"
#include "Color.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

//---------------------------------

class Primitive
{
public:
	Primitive();
	virtual ~Primitive();

	virtual void draw(Shader* shader = nullptr) const;

	virtual void setPosition(const glm::vec3& position);
	virtual glm::vec3 getPosition() const;

	virtual void setScale(const glm::vec3& scale);
	virtual glm::vec3 getScale() const;

	virtual void setDirection(const glm::vec3& direction);
	virtual glm::vec3 getDirection() const;

	virtual void setColor(const Color& color);
	virtual Color getColor() const;

	virtual void setTexture(Texture* texture);
	virtual Texture* getTexture() const;

	virtual void setNormalMap(Texture* normalmap);
	virtual Texture* getNormalMap() const;

	virtual void setLightningEnabled(bool enable);
	virtual bool getLightningEnabled() const;

protected:
	virtual void cleanup();
	virtual void updateVertexData() = 0;
	virtual void updateTransform();
	virtual void bindShader(Shader& shader) const;

	VertexBuffer m_vertex_buffer;
	GLuint m_vertex_array;

	Texture* m_texture;
	Texture* m_normalmap;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_direction;
	glm::mat4 m_transform;
	Color m_color;
	bool m_use_lightning;

};

//---------------------------------