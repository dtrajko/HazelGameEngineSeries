#pragma once

#include <glm/glm.hpp>


class OBJLoader
{
public:
	static void loadOBJModel(std::string fileName);

private:
	static void processVertex(std::vector<std::string> vertexData, std::vector<unsigned int>& vecIndices,
		std::vector<glm::vec2>& vecTextureCoords, std::vector<glm::vec3>& vecNormals);

public:
	static float* m_VerticesArray;
	static unsigned int* m_IndicesArray;
	static float* m_NormalsArray;
	static float* m_TextureArray;
	static unsigned int m_VerticesCount;
	static unsigned int m_IndicesCount;
	static unsigned int m_NormalsCount;
	static unsigned int m_TextureCoordsCount;

};
