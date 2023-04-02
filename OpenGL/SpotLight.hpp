#pragma once

#include "Light.hpp"

//-----------------------------------

class SpotLight: public Light
{
public:
	SpotLight();

	virtual void setPosition(const glm::vec3& position);
	virtual glm::vec3 getPosition() const;

	virtual void setDirection(const glm::vec3& direction);
	virtual glm::vec3 getDirection() const;

	virtual void setCutoffAngle(float angle);
	virtual float getCutoffAngle() const;
	
	virtual void setOuterCutoffAngle(float angle);
	virtual float getOuterCutoffAngle() const;

	virtual void draw(Shader& shader) const;
	virtual void afterDraw(Shader& shader) const;

	virtual void drawDebugGui();

protected:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	float m_cutoff_angle;
	float m_outer_cutoff_angle;

private:
	static int s_counter;

};

//-----------------------------------