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



class Renderer
{
public:
	Renderer();
	bool Update();
	~Renderer();
	void Init();
	bool framebufferResized = false;
	void SetModel(Model model);

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

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	void CreateDepthResources();
	

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	VkFormat findDepthFormat();

	

	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	
	void RecreateSwapChain();

	void CleanupSwapChain();

	
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void CreateTextureImage(Texture& tex, const std::string& filename);
	void CreateTextureImageView(Texture & tex);
	void CreateTextureSampler(Texture& tex);

	void CreateTexture(Texture& tex, const std::string& filename);

	void DestroyTexture(Texture & tex);

	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	

	VkCommandBuffer beginSingleTimeCommands();

	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

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
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkInstance instance;
	VkDevice device;
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

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;


	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	/*VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;*/
	Texture texture;

	UniformComponent* uniform;


	std::vector<VkFramebuffer> swapChainFramebuffers;
	
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector < VkSemaphore> imageAvailableSemaphores;
	std::vector < VkSemaphore> renderFinishedSemaphores;

	std::vector < VkFence> inFlightFenses;
	uint32_t currentFrame = 0;

	Model model;

	CameraComponent* activeCamera;
};

