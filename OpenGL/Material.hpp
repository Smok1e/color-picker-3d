#pragma once

#include "Texture.hpp"
#include "Color.hpp"
#include "Shader.hpp"

//-----------------------------------

// Represents a set of maps and attributes of material
// Each map pointer can be set to nullptr to disable it
class Material
{
public:
	Material();
	Material(const Material& copy);
	explicit Material(const Color& color);

	// Passes material values as shader uniform
	void apply(Shader& shader) const;

	void setDiffuseMap(Texture* diffuse);
	Texture* getDiffuseMap() const;

	void setNormalMap(Texture* normal);
	Texture* getNormalMap() const;

	void setSpecularMap(Texture* specular);
	Texture* getSpecularMap() const;

	void setDepthMap(Texture* depth);
	Texture* getDepthMap() const;

	void setColor(const Color& color);
	Color getColor() const;

	void setLightningEnabled(bool enable);
	bool getLightningEnabled() const;

private:
	enum class Map {
		Diffuse,
		Normal,
		Specular,
		Depth
	};

	Texture* m_diffuse;
	Texture* m_normal;
	Texture* m_specular;
	Texture* m_depth;
	Color m_color;
	bool m_lightning_enabled;

};

//-----------------------------------