#pragma once

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>
#include <vector>
#include "TextureSystem.h"
#include <array>
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

	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
};

struct UniformComponent {
	std::vector<VkDescriptorSet> descriptorSets;
	Pipeline* pipeline;
	std::array<Texture, 5> textures;
};

struct GeometryComponent {
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	uint32_t numVerticies;
	uint32_t numIndices;
};
