#pragma once

#include "VertexBuffer.hpp"
#include "Drawable.hpp"
#include "Material.hpp"

//---------------------------------

class Primitive: public Drawable
{
public:
	Primitive();
	virtual ~Primitive();

	virtual void draw(Shader& shader) const;

	virtual void setPosition(const glm::vec3& position);
	virtual glm::vec3 getPosition() const;

	virtual void setScale(const glm::vec3& scale);
	virtual glm::vec3 getScale() const;

	virtual void setRotation(const glm::vec3& rotation);
	virtual glm::vec3 getRotation() const;

	virtual void setMaterial(Material* material);
	virtual Material* getMaterial() const;

	virtual void drawDebugGui();

protected:
	virtual void cleanup();
	virtual void calculateVertices() = 0;
	virtual void updateVertexData();
	virtual void updateTransform();
	virtual void bindShader(Shader& shader) const;

	VertexBuffer m_vertex_buffer;
	GLuint m_vertex_array;

	Material* m_material;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;
	glm::mat4 m_model;
	glm::mat3 m_normal_matrix;

};

//---------------------------------