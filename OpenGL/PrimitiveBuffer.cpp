#include "PrimitiveBuffer.hpp"
#include "Utils.hpp"

//-----------------------------------

PrimitiveBuffer::PrimitiveBuffer():
	m_objects()
{}

PrimitiveBuffer::~PrimitiveBuffer()
{
	for (auto& object: m_objects) 
		delete object;
}

//-----------------------------------

bool PrimitiveBuffer::addObject(Primitive* object)
{
	iterator iter = std::find(begin(), end(), object);
	if (iter != end())
		return false;

	m_objects.push_back(object);
	return true;
}

bool PrimitiveBuffer::deleteObject(Primitive* object)
{
	iterator iter = std::find(begin(), end(), object);
	if (iter == end())
		return false;

	m_objects.erase(iter);
	delete object;
	return true;
}

//-----------------------------------

void PrimitiveBuffer::drawObjects(Shader* shader /*= nullptr*/)
{
	for (const auto& object: m_objects)
		object -> draw(shader);
}

//-----------------------------------

PrimitiveBuffer::iterator PrimitiveBuffer::begin()
{
	return m_objects.begin();
}

PrimitiveBuffer::const_iterator PrimitiveBuffer::begin() const
{
	return m_objects.begin();
}

PrimitiveBuffer::iterator PrimitiveBuffer::end()
{
	return m_objects.end();
}

PrimitiveBuffer::const_iterator PrimitiveBuffer::end() const
{
	return m_objects.end();
}

size_t PrimitiveBuffer::size() const
{
	return m_objects.size();
}

//-----------------------------------