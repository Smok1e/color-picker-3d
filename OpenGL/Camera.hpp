#include <glm/glm.hpp>

//-----------------------------------

class Camera
{
public:
	Camera();

	void setPosition(const glm::vec3& position);
	void addPosition(const glm::vec3& position);
	void translate(const glm::vec3& direction);
	glm::vec3 getPosition() const;

	void setVelocity(const glm::vec3& velocity);
	void addVelocity(const glm::vec3& velocity);
	glm::vec3 getVelocity() const;

	void setRotation(const glm::vec2& rotation);
	void addRotation(const glm::vec2& rotation);
	glm::vec2 getRotation() const;

	void setVelocityDampling(float dampling);
	float getVelocityDampling() const;

	constexpr const glm::mat4& getView() const {
		return m_view;
	}

	void update();
	void reset();

protected:
	void updateView();
	void updateAxes();

	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec2 m_rotation;
	float m_velocity_dampling;

	glm::vec<3, glm::vec3> m_axes;
	glm::mat4 m_view;

};

//-----------------------------------