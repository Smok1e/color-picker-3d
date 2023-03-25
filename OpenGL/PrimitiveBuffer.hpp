#pragma once
#include <vector>

#include "Primitive.hpp"
#include "Light.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

//-----------------------------------

extern const unsigned LIGHTS_MAX;

//-----------------------------------

class PrimitiveBuffer
{
public:
	typedef std::vector<Primitive*>::iterator       iterator;
	typedef std::vector<Primitive*>::const_iterator const_iterator;

	PrimitiveBuffer();
	~PrimitiveBuffer();

	bool addObject(Primitive* object);
	bool addObject(Light* light);
	bool deleteObject(Primitive* object);
	bool deleteObject(Light* light);
	void drawObjects(Shader* shader = nullptr);

	template<typename primitive_t> primitive_t* operator+=(primitive_t* object);
	template<typename primitive_t> primitive_t* operator-=(primitive_t* object);

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;
	size_t size() const;

protected:
	std::vector<Primitive*> m_objects;
	std::vector<Light*>     m_lights;

};

//-----------------------------------

template<typename primitive_t> 
primitive_t* PrimitiveBuffer::operator+=(primitive_t* object)
{
	addObject(object);
	return object;
}

template <typename primitive_t>
primitive_t* PrimitiveBuffer::operator-=(primitive_t* object)
{
	removeObject(object);
	return object;
}

//-----------------------------------