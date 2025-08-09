#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <glm/glm.hpp>
#include <chrono>
#include "Components.h"
#include <entt.hpp>
#include "TextureSystem.h"
#include "RenderConstants.h"
#include <iostream>
struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};



class UniformValue {
public:
	UniformValue(VkDescriptorType dT, VkShaderStageFlags s) : descriptorType(dT), stage(s) {}
	virtual VkWriteDescriptorSet GetDescriptorSetLayoutBinding(VkDescriptorSet descriptorSet, int binding, int frame) = 0;
	VkDescriptorType GetDescriptorType() { return descriptorType; }
	VkShaderStageFlags GetStage() { return stage; }
	VkDescriptorPoolSize GetDescriptorPoolSize() {
		VkDescriptorPoolSize poolSize;
		poolSize.type = descriptorType;
		poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		return poolSize;
	}

protected:
	VkDescriptorType descriptorType;
	VkShaderStageFlags stage;
};

class UniformImageSampler : public UniformValue {
public:
	UniformImageSampler(VkShaderStageFlags s) : UniformValue(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, s) {}
	void setTexture(Texture* texture) { tex = texture; }
	VkWriteDescriptorSet GetDescriptorSetLayoutBinding(VkDescriptorSet descriptorSet, int binding, int frame) override {
		VkDescriptorImageInfo * imageInfo = new VkDescriptorImageInfo();
		imageInfo->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		if (tex) {
			imageInfo->imageView = tex->textureImageView;
			imageInfo->sampler = tex->textureSampler;
		}
		else {
			std::cout << "UniformImageSampler is missing its texture!!!\n";
		}
		

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = descriptorType;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = imageInfo;
		return descriptorWrite;
	}


private:
	Texture* tex;
};

template <typename T>
class UniformUBO : public UniformValue {
public:
	UniformUBO(VkDevice & d, VkPhysicalDevice & physicalDevice, VkShaderStageFlags s) : UniformValue(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, s), device(d) {
		VkDeviceSize bufferSize = sizeof(T) * MAX_ENTITIES;

		uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
		uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

			vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
		}
	}

	VkWriteDescriptorSet GetDescriptorSetLayoutBinding(VkDescriptorSet descriptorSet, int binding, int frame) override {
		VkDescriptorBufferInfo * bufferInfo = new VkDescriptorBufferInfo();
		bufferInfo->buffer = uniformBuffers[frame];
		bufferInfo->offset = 0;
		bufferInfo->range = sizeof(T);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = descriptorType;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = bufferInfo;
		return descriptorWrite;
	}
	void* GetMappedUniformBuffer(int currentImage) {
		return uniformBuffersMapped[currentImage];
	}
	~UniformUBO() {
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroyBuffer(device, uniformBuffers[i], nullptr);
			vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
		}
	}
private:
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;
	VkDevice& device;
};


//class Uniforms {
//public:
//	Uniforms(VkDevice & d, VkPhysicalDevice & p) : physicalDevice(p), device(d), descriptorPool(), descriptorSets(), descriptorSetLayout(){}
//
//	void Create();
//
//	void Update(uint32_t currentImage, CameraComponent* camera, TransformComponent* transform);
//
//	void CreateDescriptorSetLayout();
//	void CreateDescriptorPool();
//	void CreateDescriptorSets();
//
//	VkDescriptorSet* GetDescriptorSet(int frame);
//	VkDescriptorSetLayout* getDescriptorSetLayout();
//
//	void Destroy();
//
//	//
//
//
//private:
//
//	VkDescriptorPool descriptorPool;
//	//std::vector<VkDescriptorSet> descriptorSets;
//
//
//	VkDevice& device;
//	VkPhysicalDevice& physicalDevice;
//
//
//
//};
namespace UniformSystem {

	void Init();
	void Destroy(entt::registry & registry);

	void CreateDescriptorSets(UniformComponent* uniform);
	void CreateUniformComponent(UniformComponent * uniform);
	void CreateDescriptorPool();
	void Destroy();
	VkDescriptorSet* GetDescriptorSet(UniformComponent* uniform, unsigned int currentFrame);
};