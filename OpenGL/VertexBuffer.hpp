#pragma once

#include <vector>

#include "Vertex.hpp"

//---------------------------------

class VertexBuffer
{
public:
	typedef std::vector<Vertex>::iterator iterator;
	typedef std::vector<Vertex>::const_iterator const_iterator;

	enum class Usage {
		StreamDraw  = GL_STREAM_DRAW,  // Use StreamDraw if the data will be modified and used few times
		StaticDraw  = GL_STATIC_DRAW,  // Use StaticDraw if the data will be modified rarely and often used
		DynamicDraw = GL_DYNAMIC_DRAW, // Use DynamicDraw if the data will be modified and used often
	};

	VertexBuffer();
	~VertexBuffer();

	void setUsage(Usage usage);
	Usage getUsage() const;

	GLuint getNativeHandle() const;

	// Binds buffer as GL_ARRAY_BUFFER
	void bind() const;

	// Binds buffer with id 0 as GL_ARRAY_BUFFER
	static void Unbind();

	// Add a vertex
	void addVertex(const Vertex& vertex);

	// Same as addVertex
	void operator+=(const Vertex& vertex);

	// Returns count of vertices currently stored in video memory
	size_t getVertexCount() const;

	// Transfers the data stored in memory into video memory
	void commit(bool calculate_normals = true);

	// Clears the data currently stored in memory
	void clear();

	// Calculates normals for each triangle that buffer contains
	void calculateNormals();

	// Container interface
	size_t size() const;
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

protected:
	GLuint m_vbo_handle;
	std::vector<Vertex> m_vertices;
	size_t m_vertex_count;
	Usage m_usage;

};

//---------------------------------