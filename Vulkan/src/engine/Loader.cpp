#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Loader.h"


void Loader::loadModel()
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str()))
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
				attrib.texcoords[2 * static_cast<uint64_t>(index.texcoord_index) + 0],
				1.0f - attrib.texcoords[2 * static_cast<uint64_t>(index.texcoord_index) + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);

			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

Loader::~Loader()
{
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
