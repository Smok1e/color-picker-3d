#define _CRT_SECURE_NO_WARNINGS

#include "Model.hpp"
#include "Utils.hpp"
#include <Windows.h>

#pragma warning(disable: 4267)

//-----------------------------------

const char* LINE_DELIMITERS  = "\n\r";
const char* DATA_DELIMITERS  = " \t";
const char* INDEX_DELIMITERS = "/";

//-----------------------------------

Model::Model():
	Primitive(),
	m_vertices(),
	m_texture(nullptr)
{}

//-----------------------------------

bool Model::loadFromMemory(const std::string& data)
{
	std::vector<glm::vec3> position_data;
	std::vector<glm::vec3> normal_data;
	std::vector<Vertex> vertices;

	#define error(format, ...) {                               \
		printf("Parsing error at line #%zu: ", line_number+1); \
		printf(format, __VA_ARGS__); 						   \
		printf("Source line: '%.*s'\n", line_len, line_begin); \
		return false;                                          \
	}

	const char* line_begin = data.c_str();
	const char* line_end = StrPBRK(line_begin, LINE_DELIMITERS);
	for (size_t line_number = 0; line_end; line_number++)
	{
		unsigned line_len = static_cast<unsigned>(line_end-line_begin);

		const char* header_begin = line_begin;
		const char* header_end = StrPBRK2(header_begin, DATA_DELIMITERS, LINE_DELIMITERS, line_end);
		if (!header_end)
			error("Expected whitespace delimiter next to header\n");

		unsigned header_len = static_cast<unsigned>(header_end-header_begin);
		if (!header_len)
			continue;

		// Vertex
		if (!strncmp("v", header_begin, header_len))
		{
			glm::vec3 position(0, 0, 0);

			const char* data_begin = StrPBRKI(header_end, DATA_DELIMITERS, line_end);
			const char* data_end = StrPBRK(data_begin, DATA_DELIMITERS, line_end);
			for (size_t i = 0; i < 3; i++)
			{
				if (!data_end) 
					error("Invalid vertex position data\n");

				if (!IsFloatStr(data_begin, data_end)) 
					error("Invalid vertex coordinate #%zu (at character %zu)\n", i+1, data_begin-line_begin+1);

				position[i] = strtof(data_begin, const_cast<char**>(&data_end));
				data_end = StrPBRK(data_begin = data_end+1, DATA_DELIMITERS, line_end);
			}

			position_data.push_back(position);
		}

		// Normal
		else if (!strncmp("vn", header_begin, header_len))
		{
			glm::vec3 normal(0, 0, 0);

			const char* data_begin = StrPBRKI(header_end, DATA_DELIMITERS, line_end);
			const char* data_end = StrPBRK(data_begin, DATA_DELIMITERS, line_end);
			for (size_t i = 0; i < 3; i++)
			{
				if (!data_end) 
					error("Invalid vertex normal data\n");

				if (!IsFloatStr(data_begin, data_end)) 
					error("Invalid normal coordinate #%zu (at character %zu)\n", i+1, data_begin-line_begin+1);

				normal[i] = strtof(data_begin, const_cast<char**>(&data_end));
				data_end = StrPBRK(data_begin = data_end+1, DATA_DELIMITERS, line_end);
			}

			normal_data.push_back(normal);
		}

		// Face
		else if (!strncmp("f", header_begin, header_len))
		{
			const char* data_begin = header_end+1;
			const char* data_end = StrPBRK(data_begin, DATA_DELIMITERS, line_end);
			for (size_t vertex_index = 0; data_end; vertex_index++)
			{
				if (!data_end || data_end > line_end)
				{
					if (vertex_index >= 3) break;
					else error("Expected at least 3 vertices for face, got only %zu\n", vertex_index+1);
				}

				Vertex vertex;

				// Parsing face as 'vertex/uv/normal'
				const char* index_begin = data_begin;
				const char* index_end = StrPBRK(index_begin, INDEX_DELIMITERS, data_end);
				for (size_t component_index = 0; component_index < 3; component_index++)
				{
					if (!index_end || index_end > data_end)
						error("Expeced 3 indices for vertex, got only %zu (vertex #%zu)\n", component_index, vertex_index);

					if (strchr(INDEX_DELIMITERS, *index_begin))
					{
						index_begin++;
						continue;
					}

					unsigned index = strtoul(index_begin, const_cast<char**>(&index_end), 10);

					// Position index
					if (component_index == 0)
					{
						if (!index || (index-1) >= position_data.size())
							error("Vertex position index is out of range. Expected from 1 to %zu, got %lu\n", position_data.size(), index);

						// In .obj file indices are starting from 1
						vertex.position = position_data[index-1];
					}

					// Normal index
					else if (component_index == 11)
					{
						if (!index || (index-1) >= normal_data.size())
							error("Vertex normal index is out of range. Expected from 1 to %zu, got %lu\n", normal_data.size(), index);

						if ((index-1) >= normal_data.size())
							index = normal_data.size();

						vertex.normal = normal_data[index-1];
					}

					index_end = StrPBRK(index_begin = index_end+1, INDEX_DELIMITERS, data_end);
				}

				// We are storing vertices as triangles, so if an extra vertex is in face, we have to repeat last 2 vertices
				if (vertex_index > 2)
				{
					// Now we are sure that at least 3 vertices are stored already, so we haven't check the size
					Vertex vertex1 = vertices[vertices.size()-3];
					Vertex vertex2 = vertices[vertices.size()-1];
					vertices.push_back(vertex1);
					vertices.push_back(vertex2);
					vertices.push_back(vertex);
				}
				else vertices.push_back(vertex);

				data_end = StrPBRK2(data_begin = data_end+1, DATA_DELIMITERS, LINE_DELIMITERS, line_end);
			}
		}

		line_begin = StrPBRKI(line_end, LINE_DELIMITERS);
		if (!line_begin)
			break;

		line_end = StrPBRK(line_begin, LINE_DELIMITERS);
	}

	#undef error

	m_vertices = vertices;
	m_vertex_count = vertices.size();
	updateVertexData();

	return true;
}

//-----------------------------------

bool Model::loadFromStream(std::ifstream& stream)
{
	std::vector<char> buffer;
	ReadStream(stream, &buffer);
	return loadFromMemory(buffer.data());
}

//-----------------------------------

bool Model::loadFromFile(const std::filesystem::path& filename)
{
	std::ifstream stream;
	stream.open(filename, std::ios::binary);
	if (!stream)
	{
		printf("Failed to open model file '%s'\n", filename.string().c_str());
		return false;
	}

	bool result = loadFromStream(stream);

	stream.close();
	return result;	
}

//-----------------------------------

void Model::updateVertexData()
{
	cleanup();

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof Vertex * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	Vertex::InitAttributes();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_vertex_count = m_vertices.size();
}

//-----------------------------------

void Model::draw(Shader* shader /*= nullptr*/) const
{
	Primitive::draw(shader);
	glBindVertexArray(m_vertex_array);
	glDrawArrays(GL_TRIANGLES, 0, m_vertex_count);
	glBindVertexArray(0);
}

//-----------------------------------