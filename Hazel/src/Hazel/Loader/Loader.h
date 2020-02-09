#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <vector>


struct Vertex
{
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec2 texCoord;

	bool operator==(const Vertex& other) const
	{
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}
};

namespace std
{
	template<> struct hash<Vertex>
	{
		size_t operator()(Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec4>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}


class Loader
{
public:

	Loader();

	static std::vector<char> readFile(const std::string& filename);

	// Models
	void loadModel(const std::string filepath);

	~Loader();

public:

	// Model data
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	uint32_t m_verticesArrSize;
	float* m_verticesArr = nullptr;
	uint32_t m_indicesArrSize;
	uint32_t* m_indicesArr = nullptr;
};
