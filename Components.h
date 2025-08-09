#pragma once

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>
#include <vector>
#include "TextureSystem.h"
//struct GraphicsComponent {
//
//};
class Pipeline;
struct TransformComponent {
	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec3 rotation;

	glm::mat4 modelMatrix;
};

struct CameraComponent {
	glm::mat4 view;
	glm::mat4 proj;
};

struct UniformComponent {
	std::vector<VkDescriptorSet> descriptorSets;
	Pipeline* pipeline;
	Texture texture;
};

struct GeometryComponent {
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	uint32_t numVerticies;
	uint32_t numIndices;
};
