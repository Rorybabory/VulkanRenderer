#include "UniformSystem.h"

#include "RenderConstants.h"

#include "VulkanHelpers.h"

#include <array>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Components.h"

namespace UniformSystem {
	VkDevice* device;
	VkPhysicalDevice* physicalDevice;
	VkDescriptorSetLayout descriptorSetLayout;

}

void UniformSystem::Init(VkDevice* d, VkPhysicalDevice * physD) {
	device = d;
	physicalDevice = physD;
	CreateDescriptorSetLayout();
}
void UniformSystem::DestroyComponent(UniformComponent* uniform) {
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyBuffer(*device, uniform->uniformBuffers[i], nullptr);
		vkFreeMemory(*device, uniform->uniformBuffersMemory[i], nullptr);
	}
	vkDestroyDescriptorPool(*device, uniform->descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(*device, uniform->descriptorSetLayout, nullptr);

}
void UniformSystem::Destroy(entt::registry& registry) {
	auto view = registry.view<UniformComponent>();

	for (auto [entity, uniform] : view.each()) {
		DestroyComponent(&uniform);
	}
}
void UniformSystem::CreateUniformComponent(UniformComponent* uniform, Texture & texture) {
	CreateUniformBuffers(uniform);
	CreateDescriptorPool(uniform);
	CreateDescriptorSets(uniform, texture);
}

void UniformSystem::CreateDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0; //layout(binding = 0)
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1; //if its an array this is the number of elements
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; //using vertex shader

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1; //layout(binding = 1)
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; //using frag shader

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 2;
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(*device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		std::cout << "failed to create descriptor set layout" << "\n";
	}
}

void UniformSystem::CreateUniformBuffers(UniformComponent* uniform) {
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniform->uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	uniform->uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	uniform->uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		CreateBuffer(*device, *physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniform->uniformBuffers[i], uniform->uniformBuffersMemory[i]);

		vkMapMemory(*device, uniform->uniformBuffersMemory[i], 0, bufferSize, 0, &uniform->uniformBuffersMapped[i]);
	}
}

void UniformSystem::UpdateUniformBuffer(UniformComponent* uniform, uint32_t currentImage, CameraComponent* camera, TransformComponent* transform) {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};
	ubo.model = glm::identity<glm::mat4>();
	//ubo.model = glm::scale(ubo.model, glm::vec3(1.0f));
	ubo.model = glm::rotate(ubo.model, time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ubo.view = camera->view;
	ubo.proj = camera->proj;

	//flip y axis, as glm is designed for opengl where y is flipped
	ubo.proj[1][1] *= -1;

	memcpy(uniform->uniformBuffersMapped[currentImage], &ubo, sizeof(UniformBufferObject));
}


void UniformSystem::CreateDescriptorPool(UniformComponent* uniform) {
	std::array<VkDescriptorPoolSize, 2> poolSize{};
	poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	poolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSize[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSize.size());
	poolInfo.pPoolSizes = poolSize.data();
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(*device, &poolInfo, nullptr, &uniform->descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}
void UniformSystem::CreateDescriptorSets(UniformComponent * uniform, Texture & texture) {
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = uniform->descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	uniform->descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(*device, &allocInfo, uniform->descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniform->uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = texture.textureImageView;
		imageInfo.sampler = texture.textureSampler;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = uniform->descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = uniform->descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pBufferInfo = &bufferInfo;
		descriptorWrites[1].pImageInfo = &imageInfo;


		vkUpdateDescriptorSets(*device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}


}

VkDescriptorSet* UniformSystem::GetDescriptorSet(UniformComponent* uniform, int frame) { return &uniform->descriptorSets[frame]; }

VkDescriptorSetLayout* UniformSystem::getDescriptorSetLayout() {
	return &descriptorSetLayout;
}