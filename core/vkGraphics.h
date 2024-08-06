#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <limits> 
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <set>
#include <vector>
#include <cstring>
#include <cstdlib>
#include "filesystem/file_helpers.h"
#include "core/globals.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool _USE_VK_VALIDATION_LAYERS = false;
#else
const bool _USE_VK_VALIDATION_LAYERS = true;
#endif


struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() const {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities{};
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


class MainApplication {
	public:
	void Run();

	private: 

	GLFWwindow* window = nullptr;
	VkInstance instance = nullptr;
	VkDebugUtilsMessengerEXT debugMessenger = nullptr;
	VkPhysicalDevice physicalDevice = nullptr;
	VkDevice logicalDevice = nullptr;
	VkQueue graphicsQueue = nullptr;
	VkSurfaceKHR surface = nullptr;
	VkQueue presentQueue = nullptr;
	VkSwapchainKHR swapChain = nullptr;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
	VkExtent2D swapChainExtent{};
	std::vector<VkImageView> swapChainImageViews;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	VkPipeline graphicsPipeline = nullptr;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool = nullptr;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	uint32_t currentFrame = 0;
	bool framebufferResized = false;

	void initWindow();
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	void initVulkan();
	void mainLoop();
	void drawFrame();
	void cleanup();
	void createSurface();
	void createLogicalDevice();
	void createSwapChain();
	void cleanupSwapChain();
	void recreateSwapChain();
	void createImageViews();
	void createRenderPass();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void createSyncObjects();
	VkShaderModule createShaderModule(const std::vector<char>& code);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;
	void createInstance();
	bool isDeviceSuitable(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	void selectPhysicalDevice();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
};