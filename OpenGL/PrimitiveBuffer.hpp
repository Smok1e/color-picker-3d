#pragma once
#include <vector>

#include "Primitive.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

//-----------------------------------

class PrimitiveBuffer
{
public:
	typedef std::vector<Primitive*>::iterator       iterator;
	typedef std::vector<Primitive*>::const_iterator const_iterator;

	PrimitiveBuffer();
	~PrimitiveBuffer();

	bool addObject(Primitive* object);
	bool deleteObject(Primitive* object);
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