#include "UniformSystem.h"

#include "RenderConstants.h"

#include "VulkanHelpers.h"

#include <array>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Components.h"
#include "RendererGlobals.h"
namespace UniformSystem {
}

void UniformSystem::Init() {
	CreateDescriptorPool();
}

void UniformSystem::CreateDescriptorPool() {
	std::vector<VkDescriptorPoolSize> poolSizes{
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, POOL_MAX_UBO },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, POOL_MAX_SAMPLERS },
	};
	
	VkDescriptorPoolSize pSize{};

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)*POOL_MAX_SETS;

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void UniformSystem::CreateUniformComponent(UniformComponent* uniform) {

}

void UniformSystem::Destroy() {
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	//vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

}
void UniformSystem::Destroy(entt::registry& registry) {
	/*auto view = registry.view<Uniforms>();

	for (auto [entity, uniform] : view.each()) {
		DestroyComponent(&uniform);
	}*/
}

VkDescriptorSet* UniformSystem::GetDescriptorSet(UniformComponent* uniform, unsigned int currentFrame) { return &uniform->descriptorSets[currentFrame]; }