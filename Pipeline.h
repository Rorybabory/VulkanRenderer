#pragma once
#include "VulkanHelpers.h"
#include "UniformSystem.h"
#include "Components.h"
#include <iostream>
class Pipeline
{
public:
	void CreatePipeline(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkShaderModule& vertShaderModule, VkShaderModule& fragShaderModule);

	Pipeline() : pipeline(), pipelineLayout() {}
	VkPipeline GetPipeline() {
		if (created) {
			return pipeline;
		}else {
			std::cout << "Pipeline not yet created\n";
			return VkPipeline();
		}
	}
	VkPipelineLayout GetLayout() {
		if (created) {
			return pipelineLayout;
		}
		else {
			std::cout << "Pipeline not yet created\n";
			return VkPipelineLayout();
		}
	}

	void Update(int currentImage, TransformComponent * transform, CameraComponent * camera, int index);

	void Destroy();
	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	std::vector<UniformValue*> uniformValues;
	VkDescriptorSetLayout descriptorSetLayout;

private:
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
	Texture texture;

	VkDescriptorPool descriptorPool;



	bool created = false;
};

