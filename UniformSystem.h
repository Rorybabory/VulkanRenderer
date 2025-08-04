#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <glm/glm.hpp>
#include <chrono>
#include "Components.h"
#include <entt.hpp>

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};


namespace UniformSystem {

	void Init(VkDevice* d, VkPhysicalDevice* physD);
	void Destroy(entt::registry & registry);
	void DestroyComponent(UniformComponent * uniform);
	void CreateDescriptorSetLayout();
	void CreateUniformBuffers(UniformComponent * uniform);
	void UpdateUniformBuffer(UniformComponent* uniform, uint32_t currentImage, CameraComponent* camera, TransformComponent* transform);
	void CreateDescriptorPool(UniformComponent* uniform);
	void CreateDescriptorSets(UniformComponent* uniform, VkImageView textureImageView, VkSampler textureSampler);

	void CreateUniformComponent(UniformComponent* uniform, VkImageView textureImageView, VkSampler textureSampler);

	VkDescriptorSet* GetDescriptorSet(UniformComponent* uniform, int frame);

	VkDescriptorSetLayout* getDescriptorSetLayout();

};