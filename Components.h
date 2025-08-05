#pragma once

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>
#include <vector>
//struct GraphicsComponent {
//
//};

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
	VkDescriptorSetLayout descriptorSetLayout;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

};

struct MaterialComponent {

};