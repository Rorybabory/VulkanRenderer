#pragma once
#include "VulkanHelpers.h"
#include "UniformSystem.h"
#include <iostream>
class Pipeline
{
public:
	void CreatePipeline(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkShaderModule& vertShaderModule, VkShaderModule& fragShaderModule);
	Pipeline(VkDevice& d) : device(d), pipeline(), pipelineLayout() {}
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
	void Destroy();
private:
	VkDevice& device;
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
	bool created = false;
};

