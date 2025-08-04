#include "AssetManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

Model AssetManager::LoadModel(std::string filename) {
	Model model;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	std::string materialDir = "./materials/";
	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), materialDir.c_str(), true);

	if (!err.empty()) {
		throw std::runtime_error(err);
	}

	if (!success) {
		throw std::runtime_error("Failed to load model");
	}

	std::cout << "Loaded Model: " << filename << "\n";

	std::unordered_map<Vertex, uint32_t> unique_vertices{};

	for (const tinyobj::shape_t& shape : shapes) {
		for (int i = 0; i < shape.mesh.indices.size(); i++) {
			tinyobj::index_t index = shape.mesh.indices[i];
			Vertex vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0], 
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};
			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f-attrib.texcoords[2 * index.texcoord_index + 1]
			};
			vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);
			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};
			if (unique_vertices.count(vertex) == 0) { //vertex is not prevelant in vertices list
				unique_vertices[vertex] = static_cast<uint32_t>(model.vertices.size());
				model.vertices.push_back(vertex);
			}

			model.indices.push_back(unique_vertices[vertex]);
		}
	}

	return model;
}