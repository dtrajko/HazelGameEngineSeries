#include "hzpch.h"

#include "Loader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <unordered_map>


void Loader::loadModel(const std::string filepath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
	{
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex = {};

			vertex.pos =
			{
				attrib.vertices[3 * static_cast<uint64_t>(index.vertex_index) + 0],
				attrib.vertices[3 * static_cast<uint64_t>(index.vertex_index) + 1],
				attrib.vertices[3 * static_cast<uint64_t>(index.vertex_index) + 2],
			};

			vertex.texCoord =
			{
				index.texcoord_index >= 0 ? attrib.texcoords[2 * static_cast<uint64_t>(index.texcoord_index) + 0] : 0,
				index.texcoord_index >= 0 ? 1.0f - attrib.texcoords[2 * static_cast<uint64_t>(index.texcoord_index) + 1] : 0,
			};

			vertex.color = { 1.0f, 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);

			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}

	uint32_t vertexStride = 3 + 4 + 2;
	uint32_t m_verticesArrSize = (uint32_t)vertices.size();
	float* m_VerticesArr = new float[m_verticesArrSize * vertexStride];
	uint32_t m_IndicesArrSize = (uint32_t)indices.size();
	uint32_t* m_IndicesArr = new uint32_t[m_IndicesArrSize];

	size_t indexVertex = 0;
	for (std::size_t i = 0; i < vertices.size(); ++i)
	{
		m_VerticesArr[indexVertex + 0] = vertices[i].pos.x;
		m_VerticesArr[indexVertex + 1] = vertices[i].pos.y;
		m_VerticesArr[indexVertex + 2] = vertices[i].pos.z;
		m_VerticesArr[indexVertex + 3] = vertices[i].color.r;
		m_VerticesArr[indexVertex + 4] = vertices[i].color.g;
		m_VerticesArr[indexVertex + 5] = vertices[i].color.b;
		m_VerticesArr[indexVertex + 6] = vertices[i].color.a;
		m_VerticesArr[indexVertex + 7] = vertices[i].texCoord.x;
		m_VerticesArr[indexVertex + 8] = vertices[i].texCoord.y;
		indexVertex += vertexStride;
	}

	for (std::size_t i = 0; i < indices.size(); ++i)
	{
		m_IndicesArr[i] = indices[i];
	}
}

Loader::Loader()
{
}

std::vector<char> Loader::readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();
	return buffer;
}

Loader::~Loader()
{
}
