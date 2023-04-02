#pragma once
#include <vector>

#include "Drawable.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

//-----------------------------------

extern const unsigned LIGHTS_MAX;

//-----------------------------------

class SceneBuffer
{
public:
	typedef std::vector<Drawable*>::iterator       iterator;
	typedef std::vector<Drawable*>::const_iterator const_iterator;

	SceneBuffer();
	~SceneBuffer();

	bool addObject(Drawable* object);
	bool deleteObject(Drawable* object);
	void drawObjects(Shader& shader) const;
	void drawDebugGui();

	template<typename primitive_t> primitive_t* operator+=(primitive_t* object);
	template<typename primitive_t> primitive_t* operator-=(primitive_t* object);

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;
	size_t size() const;

protected:
	std::vector<Drawable*> m_objects;

};

//-----------------------------------

template<typename primitive_t> 
primitive_t* SceneBuffer::operator+=(primitive_t* object)
{
	addObject(object);
	return object;
}

template <typename primitive_t>
primitive_t* SceneBuffer::operator-=(primitive_t* object)
{
	deleteObject(object);
	return object;
}

//-----------------------------------