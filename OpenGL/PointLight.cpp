#define _CRT_SECURE_NO_WARNINGS

#include "PointLight.hpp"

//---------------------------------

PointLight::PointLight():
	Light(),
	m_position(glm::vec3(0, 0, 0))
{}

int PointLight::s_counter = 0;

//---------------------------------

void PointLight::setPosition(const glm::vec3& position)
{
	m_position = position;
}

glm::vec3 PointLight::getPosition() const
{
	return m_position;
}

//---------------------------------
 
void PointLight::draw(Shader& shader) const
{
	s_counter += (s_counter == -1);

	//auto start = std::chrono::high_resolution_clock::now();
	//for (size_t i = 0; i < 10000; i++)
	//{
	//	static char instance[BUFFSIZE] = "";
	//	static char field[BUFFSIZE] = "";
	//
	//	sprintf_s(instance, "pointLight[%d].", s_counter);
	//	shader.setUniform(strcat(field, ".position"         ), m_position         );
	//	shader.setUniform(strcat(field, ".color"            ), m_color            );
	//	shader.setUniform(strcat(field, ".ambient_strength" ), m_ambient_strength );
	//	shader.setUniform(strcat(field, ".diffuse_strength" ), m_diffuse_strength );
	//	shader.setUniform(strcat(field, ".specular_strength"), m_specular_strength);
	//}
	//
	//auto elapsed = std::chrono::high_resolution_clock::now() - start;
	//printf("Elapsed %lld milliseconds\n", std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());
	//DebugBreak();

	// 609 ms - setUniformFormatted
	// 643 ms - operator+


	s_counter++;
}

void PointLight::afterDraw(Shader& shader) const
{
	if (s_counter != -1)
	{
		shader["pointLightCount"] = s_counter;
		s_counter = -1;
	}
}

//---------------------------------