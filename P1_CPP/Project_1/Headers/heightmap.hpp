#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstddef>
#include <iostream>
#include <vector>

#include <shader.hpp>

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
#include <stb_image.h>

struct Vertex {
	// position
	glm::vec3 Position;
	// texCoords
	glm::vec2 TexCoords;
};

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Heightmap
{
public:
	int width;
	int height;
	unsigned int VAO;

	// pointer to data - data is an array so can be accessed by data[x]. 
	//       - this is an uint8 array (so values range from 0-255)
	unsigned char *data;

	// heightmap vertices
	std::vector<Vertex> vertices;
	// indices for EBO
	std::vector<unsigned int> indices;

	// heightmapPath is the path to the heightmap image file
	// terrainSize is the size of the terrain in world units
	// heightScale is the scale factor for the height values
	// baseHeight is the base height of the terrain
	Heightmap(const char* heightmapPath, float terrainSize = 200.0f,
		float heightScale = 12.0f, float baseHeight = -48.0f)
		: width(0), height(0), VAO(0), data(nullptr), VBO(0), EBO(0),
		  terrainSize(terrainSize), heightScale(heightScale), baseHeight(baseHeight)
	{
		load_heightmap(heightmapPath);
		if (data == nullptr || width < 2 || height < 2)
		{
			if (data != nullptr)
				stbi_image_free(data);
			data = nullptr;
			std::cerr << "Heightmap must be a readable image at least 2x2 pixels: "
				<< heightmapPath << std::endl;
			return;
		}

		create_heightmap();
		create_indices();
		stbi_image_free(data);
		data = nullptr;
		setup_heightmap();
	}

	~Heightmap()
	{
		Cleanup();
	}

	void Cleanup()
	{
		if (EBO != 0)
		{
			glDeleteBuffers(1, &EBO);
			EBO = 0;
		}
		if (VBO != 0)
		{
			glDeleteBuffers(1, &VBO);
			VBO = 0;
		}
		if (VAO != 0)
		{
			glDeleteVertexArrays(1, &VAO);
			VAO = 0;
		}
	}

	Heightmap(const Heightmap&) = delete;
	Heightmap& operator=(const Heightmap&) = delete;

	void Draw(const Shader& shader, unsigned int textureID) const
	{
		if (VAO == 0 || indices.empty())
			return;

		// The fragment shader blends texture units 0 and 1, so bind the chosen
		// terrain texture to both units to display it without mixing other images.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureID);

		shader.setMat4("model", glm::mat4(1.0f));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
			GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

private:
	unsigned int VBO;
	unsigned int EBO;
	float terrainSize;
	float heightScale;
	float baseHeight;

	void load_heightmap(const char* heightmapPath)
	{
		int channels = 0;
		// Request one channel so each sample is a consistent grayscale intensity.
		data = stbi_load(heightmapPath, &width, &height, &channels, 1);
		if (data == nullptr)
			std::cerr << "Failed to load heightmap: " << heightmapPath << std::endl;
	}

	void create_heightmap()
	{
		vertices.reserve(static_cast<std::size_t>(width) * static_cast<std::size_t>(height));
		for (int row = 0; row < height; ++row)
		{
			for (int column = 0; column < width; ++column)
			{
				const float u = static_cast<float>(column) / static_cast<float>(width - 1);
				const float v = static_cast<float>(row) / static_cast<float>(height - 1);
				const float intensity = static_cast<float>(data[row * width + column]) / 255.0f;

				Vertex vertex;
				vertex.Position = glm::vec3(
					(u - 0.5f) * terrainSize,
					baseHeight + intensity * heightScale,
					(0.5f - v) * terrainSize);
				vertex.TexCoords = glm::vec2(u, v);
				vertices.push_back(vertex);
			}
		}
	}

	void create_indices()
	{
		indices.reserve(static_cast<std::size_t>(width - 1) *
			static_cast<std::size_t>(height - 1) * 6);
		for (int row = 0; row < height - 1; ++row)
		{
			for (int column = 0; column < width - 1; ++column)
			{
				const unsigned int topLeft = static_cast<unsigned int>(row * width + column);
				const unsigned int topRight = topLeft + 1;
				const unsigned int bottomLeft = topLeft + static_cast<unsigned int>(width);
				const unsigned int bottomRight = bottomLeft + 1;

				// Counter-clockwise winding as viewed from above (+Y).
				indices.push_back(topLeft);
				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);
				indices.push_back(bottomRight);
				indices.push_back(bottomLeft);
			}
		}
	}

	void setup_heightmap()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
			vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
			indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, Position)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}

};
#endif