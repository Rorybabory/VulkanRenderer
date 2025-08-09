#pragma once
#include <vulkan/vulkan.h>
#include <string>
struct Texture {
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;
	uint32_t mipLevels;
};
