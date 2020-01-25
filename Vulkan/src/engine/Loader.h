#pragma once


#include <vector>

#include "Vertex.h"


class Loader
{
public:

	Loader();

	static std::vector<char> readFile(const std::string& filename);

	// Models
	void loadModel();

	~Loader();

public:

	const std::string MODEL_PATH = "models/cube.obj";
	const std::string TEXTURE_PATH = "textures/statue_512x512_atlas.png";

	const std::vector<Vertex> vertices_triangle =
	{
		{ { 0.0f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
		{ { 0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
		{ {-0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
	};

	const std::vector<Vertex> vertices_quads = {
		{ {-0.5f, -0.5f, 0.0f}, { 1.0f, 0.0f, 0.0f }, {0.0f, 0.0f} },
		{ { 0.5f, -0.5f, 0.0f}, { 0.0f, 1.0f, 0.0f }, {1.0f, 0.0f} },
		{ { 0.5f,  0.5f, 0.0f}, { 0.0f, 0.0f, 1.0f }, {1.0f, 1.0f} },
		{ {-0.5f,  0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f }, {0.0f, 1.0f} },

		{ {-0.5f, -0.5f, 0.5f}, { 1.0f, 0.0f, 0.0f }, {0.0f, 0.0f} },
		{ { 0.5f, -0.5f, 0.5f}, { 0.0f, 1.0f, 0.0f }, {1.0f, 0.0f} },
		{ { 0.5f,  0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f }, {1.0f, 1.0f} },
		{ {-0.5f,  0.5f, 0.5f}, { 1.0f, 1.0f, 1.0f }, {0.0f, 1.0f} }
	};

	const std::vector<uint32_t> indices_quads = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
	};

	// Model data
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

};
