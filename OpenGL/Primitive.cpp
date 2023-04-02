#define _USE_MATH_DEFINES

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include "Primitive.hpp"
#include "Utils.hpp"

//---------------------------------

Primitive::Primitive():
	m_vertex_buffer(),
	m_vertex_array(0),
	m_position(0),
	m_scale(glm::vec3(1, 1, 1)),
	m_rotation(0),
	m_model(glm::identity<glm::mat4>()),
	m_material(nullptr)
{
	updateTransform();
}

Primitive::~Primitive()
{
	cleanup();
}

//---------------------------------

void Primitive::draw(Shader& shader) const
{
	bindShader(shader);
	glSafeCallVoid(glBindVertexArray(m_vertex_array));
	glSafeCallVoid(glDrawArrays(GL_TRIANGLES, 0, m_vertex_buffer.getVertexCount()));
	glSafeCallVoid(glBindVertexArray(0));
}

//---------------------------------

void Primitive::setPosition(const glm::vec3& position)
{
	if (m_position != position) 
		m_position = position, updateTransform();
}

glm::vec3 Primitive::getPosition() const
{
	return m_position;
}

void Primitive::setScale(const glm::vec3& scale)
{
	if (m_scale != scale)
		m_scale = scale, updateTransform();
}

glm::vec3 Primitive::getScale() const
{
	return m_scale;
}

void Primitive::setRotation(const glm::vec3& rotation)
{
	if (m_rotation != rotation)
		m_rotation = rotation, updateTransform();
}

glm::vec3 Primitive::getRotation() const
{
	return m_rotation;
}

void Primitive::setMaterial(Material* material)
{
	m_material = material;
}

Material* Primitive::getMaterial() const
{
	return m_material;
}

//---------------------------------

void Primitive::updateTransform()
{
	m_model = glm::identity<glm::mat4>();

	// Position
	m_model = glm::translate(m_model, m_position);

	// Rotation
	m_model = glm::rotate(m_model, m_rotation.x, glm::vec3(1, 0, 0));
	m_model = glm::rotate(m_model, m_rotation.y, glm::vec3(0, 1, 0));
	m_model = glm::rotate(m_model, m_rotation.z, glm::vec3(0, 0, 1));

	// Scale
	m_model = glm::scale(m_model, m_scale);

	// Inverted and transposed matrix for normal calculation
	m_normal_matrix = glm::transpose(glm::inverse(glm::mat3(m_model)));
}

void Primitive::updateVertexData()
{
	cleanup();
	calculateVertices();
	m_vertex_buffer.bind();
	glSafeCallVoid(glGenVertexArrays(1, &m_vertex_array));
	glSafeCallVoid(glBindVertexArray(m_vertex_array));
	Vertex::InitAttributes();
	VertexBuffer::Unbind();
	glSafeCallVoid(glBindVertexArray(0));
}

//---------------------------------

void Primitive::cleanup()
{
	if (m_vertex_array) glSafeCallVoid(glDeleteVertexArrays(1, &m_vertex_array));
	m_vertex_array = 0;
}

void Primitive::bindShader(Shader& shader) const
{
	shader["model"            ] = m_model;
	shader["modelNormalMatrix"] = m_normal_matrix;
	if (m_material) m_material->apply(shader);
	shader.use();
}

//---------------------------------

void Primitive::drawDebugGui()
{
	ImGui::Text("This is drawable %p.", this);
}

//---------------------------------