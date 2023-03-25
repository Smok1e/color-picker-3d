#pragma once
#include "Primitive.hpp"
#include "Texture.hpp"

//-----------------------------------

class Model: public Primitive
{
public:
	Model();

	virtual void draw(Shader* shader = nullptr) const;

	virtual bool loadFromMemory(const std::string& data);
	virtual bool loadFromStream(std::ifstream& stream);
	virtual bool loadFromFile(const std::filesystem::path& filename);

protected:
	virtual void updateVertexData();

	std::vector<Vertex> m_vertices;
	Texture* m_texture;

};

//-----------------------------------