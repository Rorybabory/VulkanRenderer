#pragma once

#include <vulkan/vulkan.h>
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>
#include <vector>
#include <optional>
#include <glm/glm.hpp>
#include "Geometry.h"
#include "UniformSystem.h"
#include "Pipeline.h"
#include "TextureSystem.h"
#include <entt.hpp>

typedef struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
}QueueFamilyIndices;

typedef struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
}SwapChainSupportDetails;

struct DrawGroup {
	GeometryComponent* geometry;
	TransformComponent* transform;
	UniformComponent* uniform;
};


class Renderer
{
public:
	Renderer(entt::registry & reg);
	bool Update();
	~Renderer();
	void Init();
	void CreateTexture(Texture& tex, const std::string& filename);
	void CreateGeometry(GeometryComponent& geo, std::string filename);
	Pipeline& GetMainPipeline() { return pipeline; }

	bool framebufferResized = false;

private:

	void Render();

	void InitWindow();
	void InitVulkan();
	void CreateInstance();
	void CreateLogicalDevice();
	void SetupDebugMessenger();
	void CreateSurface();
	

	void PickPhysicalDevice();

	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateSwapChain();
	void CreateImageViews();
	void CreateGraphicsPipeline();
	void CreateRenderPass();
	void CreateFramebuffers();
	void CreateCommandBuffers();
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void CreateCommandPool();

	void CreateSyncObjects();

	void CreateVertexBuffer(GeometryComponent& geo, Model & m);
	void CreateIndexBuffer(GeometryComponent& geo, Model& m);

	void DestroyGeometry(GeometryComponent& geo);

	void CreateDepthResources();
	

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	VkFormat findDepthFormat();

	

	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, std::vector<DrawGroup>& drawGroup);
	
	void RecreateSwapChain();

	void CleanupSwapChain();


	
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void CreateTextureImage(Texture& tex, const std::string& filename);
	void CreateTextureImageView(Texture & tex);
	void CreateTextureSampler(Texture& tex);


	void DestroyTexture(Texture & tex);

	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	

	VkCommandBuffer beginSingleTimeCommands();

	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);


	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);



	GLFWwindow* window;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkInstance instance;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSurfaceKHR surface;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	VkRenderPass renderPass;

	Pipeline pipeline;

	GeometryComponent geometry;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	/*VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;*/



	std::vector<VkFramebuffer> swapChainFramebuffers;
	
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector < VkSemaphore> imageAvailableSemaphores;
	std::vector < VkSemaphore> renderFinishedSemaphores;

	std::vector < VkFence> inFlightFenses;
	uint32_t currentFrame = 0;

	entt::registry& registry;

	CameraComponent* activeCamera;
};

