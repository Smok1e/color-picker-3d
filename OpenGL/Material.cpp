#include "Material.hpp"

//-----------------------------------

Material::Material():
	m_diffuse(nullptr),
	m_normal(nullptr),
	m_specular(nullptr),
	m_depth(nullptr),
	m_color(Color::Black),
	m_lightning_enabled(true)
{}

Material::Material(const Material& copy):
	m_diffuse(copy.m_diffuse),
	m_normal(copy.m_normal),
	m_specular(copy.m_specular),
	m_depth(copy.m_depth),
	m_color(copy.m_color),
	m_lightning_enabled(copy.m_lightning_enabled)
{}

Material::Material(const Color& color):
	m_diffuse(nullptr),
	m_normal(nullptr),
	m_specular(nullptr),
	m_depth(nullptr),
	m_color(color),
	m_lightning_enabled(true)
{}

//-----------------------------------

void Material::apply(Shader& shader) const
{
	shader["materialUseDiffuseMap" ] = m_diffuse  != nullptr;
	shader["materialUseNormalMap"  ] = m_normal   != nullptr;
	shader["materialUseSpecularMap"] = m_specular != nullptr;
	//shader["materialUseDepthMap"   ] = m_depth    != nullptr;
	shader["materialUseLightning"  ] = m_lightning_enabled;
	shader["materialColor"         ] = m_color;
	if (m_diffuse ) shader.setUniform("materialDiffuseMap",  *m_diffuse,  static_cast<int>(Map::Diffuse ));
	if (m_normal  ) shader.setUniform("materialNormalMap",   *m_normal,   static_cast<int>(Map::Normal  ));
	if (m_specular) shader.setUniform("materialSpecularMap", *m_specular, static_cast<int>(Map::Specular));
	//if (m_depth   ) shader.setUniform("materialDepthMap",    *m_depth,  static_cast<int>(Map::Depth   ));
}

//-----------------------------------

void Material::setDiffuseMap(Texture* diffuse)
{
	m_diffuse = diffuse;
}

Texture* Material::getDiffuseMap() const
{
	return m_diffuse;
}

void Material::setNormalMap(Texture* normal)
{
	m_normal = normal;
}

Texture* Material::getNormalMap() const
{
	return m_normal;
}

void Material::setSpecularMap(Texture* specular)
{
	m_specular = specular;
}

Texture* Material::getSpecularMap() const
{
	return m_specular;
}

void Material::setDepthMap(Texture* depth)
{
	m_depth = depth;
}

Texture* Material::getDepthMap() const
{
	return m_depth;
}

void Material::setColor(const Color& color)
{
	m_color = color;
}

Color Material::getColor() const
{
	return m_color;
}

void Material::setLightningEnabled(bool enable)
{
	m_lightning_enabled = enable;
}

bool Material::getLightningEnabled() const
{
	return m_lightning_enabled;
}

//-----------------------------------