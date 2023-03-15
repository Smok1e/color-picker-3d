#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

//-----------------------------------

Camera::Camera():
	m_position(),
	m_velocity(),
	m_rotation(),
	m_axes(),
	m_view(),
	m_velocity_dampling(.9f)
{
	reset();
}

//-----------------------------------

void Camera::setPosition(const glm::vec3& position)
{
	if (position != m_position)
	{
		m_position = position;
		updateView();
	}
}

void Camera::addPosition(const glm::vec3& position)
{
	setPosition(m_position+position);
}

void Camera::translate(const glm::vec3& direction)
{
	if (direction.x != 0 || direction.y != 0 || direction.z != 0)
	{
		m_position += direction.x*m_axes.x;
		m_position += direction.y*m_axes.y;
		m_position += direction.z*m_axes.z;
		updateView();
	}
}

glm::vec3 Camera::getPosition() const
{
	return m_position;
}

//-----------------------------------

void Camera::setVelocity(const glm::vec3& velocity)
{
	m_velocity = velocity;
}

void Camera::addVelocity(const glm::vec3& velocity)
{
	m_velocity += velocity;
}

glm::vec3 Camera::getVelocity() const
{
	return m_velocity;
}

//-----------------------------------

void Camera::setRotation(const glm::vec2& rotation)
{
	if (rotation != m_rotation)
	{
		m_rotation = rotation;
		updateAxes();
		updateView();
	}
}

void Camera::addRotation(const glm::vec2& rotation)
{
	setRotation(m_rotation+rotation);
}

glm::vec2 Camera::getRotation() const
{
	return m_rotation;
}

//-----------------------------------

void Camera::setVelocityDampling(float dampling)
{
	m_velocity_dampling = dampling;
}

float Camera::getVelocityDampling() const
{
	return m_velocity_dampling;
}

//-----------------------------------

void Camera::updateView()
{
	m_view = glm::lookAt(m_position, m_position+m_axes.z, m_axes.y);
}

void Camera::updateAxes()
{
	// Z axis is camera direction, found as 3-component vector constructed from two angles
	m_axes.z = glm::normalize(glm::vec3(
		cos(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x-90)),
		sin(glm::radians(m_rotation.y)),
		cos(glm::radians(m_rotation.y)) * sin(glm::radians(m_rotation.x-90))
	));

	// Perpendicular vector of two vectors can be calculated as cross product of vectors;
	// So X axis is perpendicular between Z axis of camera (direction) and Y axis of world (basically just (0; 1; 0))
	m_axes.x = glm::normalize(glm::cross(m_axes.z, glm::vec3(0, 1, 0)));

	// And Y axis is perpendicular to Z and X axes of camera
	m_axes.y = glm::normalize(glm::cross(m_axes.x, m_axes.z));	
}

//-----------------------------------

void Camera::update()
{
	translate(m_velocity);
	m_velocity *= m_velocity_dampling;
}

void Camera::reset()
{
	m_position = glm::vec3(0, 0, 3);
	m_axes.x   = glm::vec3(1, 0, 0);
	m_axes.y   = glm::vec3(0, 1, 0);
	m_axes.z   = glm::vec3(0, 0, 1);
	m_velocity = glm::vec3(0, 0, 0);
	m_rotation = glm::vec2(0, 0   );

	updateAxes();
	updateView();
}

//-----------------------------------