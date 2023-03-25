#include "PrimitiveBuffer.hpp"
#include "Utils.hpp"

//-----------------------------------

const unsigned LIGHTS_MAX = 32;

//-----------------------------------

PrimitiveBuffer::PrimitiveBuffer():
	m_objects(),
	m_lights()
{}

PrimitiveBuffer::~PrimitiveBuffer()
{
	for (auto& object: m_objects) delete object;
	for (auto& light:  m_lights ) delete light;
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

bool PrimitiveBuffer::addObject(Light* light)
{
	auto iter = std::find(m_lights.begin(), m_lights.end(), light);
	if (iter != m_lights.end())
		return false;

	m_lights.push_back(light);
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

bool PrimitiveBuffer::deleteObject(Light* light)
{
	auto iter = std::find(m_lights.begin(), m_lights.end(), light);
	if (iter == m_lights.end())
		return false;

	m_lights.erase(iter);
	delete light;
	return true;
}

//-----------------------------------

void PrimitiveBuffer::drawObjects(Shader* shader /*= nullptr*/)
{
	if (shader)
	{
		for (size_t i = 0, lights_count = m_lights.size(); i < lights_count && i < LIGHTS_MAX; i++)
		{
			Light* light = m_lights[i];
			shader->setUniformFormatted(light->getPosition(),         "lights[%zu].position", i);
			shader->setUniformFormatted(light->getColor(),            "lights[%zu].color",    i);
			shader->setUniformFormatted(light->getAmbientStrength(),  "lights[%zu].ambient",  i);
			shader->setUniformFormatted(light->getDiffuseStrength(),  "lights[%zu].diffuse",  i);
			shader->setUniformFormatted(light->getSpecularStrength(), "lights[%zu].specular", i);
		}
	}

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