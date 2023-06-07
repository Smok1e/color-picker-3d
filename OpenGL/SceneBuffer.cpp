#include "SceneBuffer.hpp"
#include "Utils.hpp"
#include "Logging.hpp"
#include "Light.hpp"

//-----------------------------------

SceneBuffer::SceneBuffer():
	m_objects()
{}

SceneBuffer::~SceneBuffer()
{
	for (auto& object: m_objects) delete object;
}

//-----------------------------------

bool SceneBuffer::addObject(Drawable* object)
{
	iterator iter = std::find(begin(), end(), object);
	if (iter != end())
		return false;

	m_objects.push_back(object);
	return true;
}

bool SceneBuffer::deleteObject(Drawable* object)
{
	iterator iter = std::find(begin(), end(), object);
	if (iter == end())
		return false;

	m_objects.erase(iter);
	delete object;
	return true;
}

//-----------------------------------

void SceneBuffer::drawObjects(Shader& shader) const
{
	for (const auto object: m_objects) object->draw(shader);
	Light::Update(shader);
}

void SceneBuffer::drawDebugGui()
{
	for (auto object: m_objects)
		object->drawDebugGui();
}

//-----------------------------------

SceneBuffer::iterator SceneBuffer::begin()
{
	return m_objects.begin();
}

SceneBuffer::const_iterator SceneBuffer::begin() const
{
	return m_objects.begin();
}

SceneBuffer::iterator SceneBuffer::end()
{
	return m_objects.end();
}

SceneBuffer::const_iterator SceneBuffer::end() const
{
	return m_objects.end();
}

size_t SceneBuffer::size() const
{
	return m_objects.size();
}

//-----------------------------------