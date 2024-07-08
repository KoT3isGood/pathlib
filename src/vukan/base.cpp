#include "base.h"


#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include "vulkan/vulkan_win32.h"
#elif defined(__linux__)
#include <X11/Xlib.h>
#include "vulkan/vulkan_xlib.h"
#include <xcb/xcb.h>
#include "vulkan/vulkan_xcb.h"
#endif
#define VK_IFUNCTION(func) PFN_##func func = (PFN_##func)vkGetInstanceProcAddr(vulkanData.instance,#func)
#define VK_DFUNCTION(func) PFN_##func func = (PFN_##func)vkGetDeviceProcAddr(vulkanData.device,#func)
#include <unordered_map>
#include <vector>

namespace vukan {
	struct VulkanData {
		VkInstance instance;
		VkDebugUtilsMessengerEXT messenger;
		VkPhysicalDevice physicalDevice;
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		std::unordered_map<void*, VkSurfaceKHR> windowToSurface;
		std::unordered_map<void*, VkSwapchainKHR> windowToSwapchain;
		std::unordered_map<void*, VkImageView[2]> windowToSwapchainImageViews;
		std::unordered_map<void*, VkImageView[2]> windowToSwapchainShaderImageViews;
		std::unordered_map<void*, int> windowToWidth;
		std::unordered_map<void*, int> windowToHeight;

		VkCommandPool commandPool;
		VkCommandBuffer commandBuffers[2];
		VkSemaphore graphicsSemaphores[2];
		VkSemaphore presentSemaphores[2];
		VkFence fences[2];
		uint32_t currentFrameIndex = 0;
	} vulkanData;


	static VkBool32 VulkanCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
		TraceLog(LogType::Rendering, "%s\n", pCallbackData->pMessage);
		return VK_FALSE;
	}

	void InitVulkan() {
		
		// Instance
		const char* instanceExtensions[] = {
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
			VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(_WIN32) || defined(_WIN64)
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined(__linux__)
			VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
      VK_KHR_XCB_SURFACE_EXTENSION_NAME
#endif
		};
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo instanceCreateInfo{};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		instanceCreateInfo.enabledExtensionCount = sizeof(instanceExtensions) / sizeof(char*);
		instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions;
		vkCreateInstance(&instanceCreateInfo, nullptr, &vulkanData.instance);


		// Debugger
		VkDebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo{};
		debugUtilsCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugUtilsCreateInfo.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugUtilsCreateInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		debugUtilsCreateInfo.pfnUserCallback = VulkanCallback;

		VK_IFUNCTION(vkCreateDebugUtilsMessengerEXT);
		vkCreateDebugUtilsMessengerEXT(vulkanData.instance, &debugUtilsCreateInfo, nullptr, &vulkanData.messenger);


		// Physical Device
		uint32_t pdcount = 0;
		vkEnumeratePhysicalDevices(vulkanData.instance, &pdcount, nullptr);
		VkPhysicalDevice* pdevices = (VkPhysicalDevice*)malloc(pdcount*sizeof(VkPhysicalDevice));
		vkEnumeratePhysicalDevices(vulkanData.instance, &pdcount, pdevices);
		vulkanData.physicalDevice = pdevices[0];
		free(pdevices);


		// Queues
		uint32_t queueCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(vulkanData.physicalDevice, &queueCount, nullptr);
		VkQueueFamilyProperties* qfp = (VkQueueFamilyProperties*)malloc(queueCount * sizeof(VkQueueFamilyProperties));
		vkGetPhysicalDeviceQueueFamilyProperties(vulkanData.physicalDevice, &queueCount, qfp);
		

		int graphicsFamily = -1;
		int presentFamily = -1;
		for (int i = 0; i < queueCount; i++) {
			if (qfp[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				graphicsFamily = i;
				presentFamily = i;
			}
			if (graphicsFamily>0 && presentFamily > 0) {
				break;
			};
		}
		free(qfp);

		// Device
		float priority = 1.0;
		VkDeviceQueueCreateInfo dqCreateInfo{};
		dqCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		dqCreateInfo.pQueuePriorities = &priority;
		dqCreateInfo.queueFamilyIndex = graphicsFamily;
		dqCreateInfo.queueCount = 1;

		const char* deviceExtensions[] = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		};
		
		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &dqCreateInfo;
		deviceCreateInfo.enabledExtensionCount = sizeof(deviceExtensions) / sizeof(char*);
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;
		vkCreateDevice(vulkanData.physicalDevice, &deviceCreateInfo,nullptr,&vulkanData.device);
		vkGetDeviceQueue(vulkanData.device, graphicsFamily, 0, &vulkanData.graphicsQueue);
		vkGetDeviceQueue(vulkanData.device, presentFamily, 0, &vulkanData.presentQueue);

		VkCommandPoolCreateInfo commandPoolCreateInfo{};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = graphicsFamily;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		vkCreateCommandPool(vulkanData.device, &commandPoolCreateInfo, nullptr, &vulkanData.commandPool);

		VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandBufferCount = 2;
		commandBufferAllocateInfo.commandPool = vulkanData.commandPool;
		vkAllocateCommandBuffers(vulkanData.device, &commandBufferAllocateInfo, vulkanData.commandBuffers);

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		for (int i = 0; i < 2; i++) {
			vkCreateSemaphore(vulkanData.device, &semaphoreCreateInfo, nullptr, &vulkanData.graphicsSemaphores[0]);
			vkCreateSemaphore(vulkanData.device, &semaphoreCreateInfo, nullptr, &vulkanData.presentSemaphores[0]);
			vkCreateFence(vulkanData.device, &fenceCreateInfo, nullptr, &vulkanData.fences[0]);

		}
	}

	void Render()
	{
		vkWaitForFences(vulkanData.device, 1, &vulkanData.fences[0], VK_TRUE, UINT64_MAX);
		vkResetFences(vulkanData.device, 1, &vulkanData.fences[0]);

		std::vector<uint32_t> imageIndexes;
		for (auto swapchains : vulkanData.windowToSwapchain) {
			uint32_t imageIndex = 0;
			VkResult result = vkAcquireNextImageKHR(vulkanData.device, swapchains.second, UINT64_MAX, vulkanData.presentSemaphores[vulkanData.currentFrameIndex], vulkanData.fences[vulkanData.currentFrameIndex], &imageIndex);
			imageIndexes.push_back(imageIndex);
			int width;
			int height;
			glfwGetWindowSize((GLFWwindow*)swapchains.first, &width, &height);
		if (vulkanData.windowToWidth[swapchains.first]!=width|| vulkanData.windowToHeight[swapchains.first] != height) {
			
			vulkanData.windowToWidth[swapchains.first] = width;
			vulkanData.windowToWidth[swapchains.first] = height;
			DestroySurface(swapchains.first);
			CreateSurface(swapchains.first);
		}
		}
		vkResetCommandBuffer(vulkanData.commandBuffers[vulkanData.currentFrameIndex], 0);
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer(vulkanData.commandBuffers[vulkanData.currentFrameIndex], &beginInfo);
		vkEndCommandBuffer(vulkanData.commandBuffers[vulkanData.currentFrameIndex]);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore waitSemaphores[] = { vulkanData.presentSemaphores[vulkanData.currentFrameIndex]};
		VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = &waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &vulkanData.commandBuffers[vulkanData.currentFrameIndex];

		VkSemaphore signalSemaphores[] = { vulkanData.graphicsSemaphores[vulkanData.currentFrameIndex] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkQueueSubmit(vulkanData.graphicsQueue, 1, &submitInfo, vulkanData.fences[vulkanData.currentFrameIndex]);
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		std::vector<VkSwapchainKHR> swapchains;
		for (auto swapchain : vulkanData.windowToSwapchain) {
			swapchains.push_back(swapchain.second);
		}
		presentInfo.swapchainCount = swapchains.size();
		presentInfo.pSwapchains = swapchains.data();
		presentInfo.pImageIndices = imageIndexes.data();
		vkQueuePresentKHR(vulkanData.presentQueue, &presentInfo);
	}

	void DeinitVulkan()
	{
	}

	VkImageView makeImageView(const VkImage& image, VkFormat format) {

		VkImageView imageView = nullptr;
		VkImageViewCreateInfo imageViewCreateInfo{};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = format;
		imageViewCreateInfo.image = image;
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		if (format == VK_FORMAT_D32_SFLOAT) {
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		}
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;
		vkCreateImageView(vulkanData.device, &imageViewCreateInfo, nullptr, &imageView);
		return imageView;
	}




	void CreateSurface(void* window)
	{
		VkSurfaceKHR surface = nullptr;
		glfwCreateWindowSurface(vulkanData.instance, (GLFWwindow*)window, nullptr, &surface);
		int width;
		int height;
		glfwGetWindowSize((GLFWwindow*)window, &width, &height);
		vulkanData.windowToWidth[window] = width;
		vulkanData.windowToHeight[window] = height;
		vulkanData.windowToSurface[window] = surface;

		VkSurfaceCapabilitiesKHR capabilies{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanData.physicalDevice, surface, &capabilies);
    VkFormat swapchainFormat;
#ifdef __linux__
		swapchainFormat = VK_FORMAT_B8G8R8A8_UNORM;
#else
    swapchainFormat = VK_FORMAT_R8G8B8A8_UNORM;
#endif

		VkSwapchainKHR swapchain = nullptr;
		VkSwapchainCreateInfoKHR swapchainCreateInfo{};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = surface;
		swapchainCreateInfo.minImageCount = 2;
		swapchainCreateInfo.imageFormat = swapchainFormat;
		swapchainCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		swapchainCreateInfo.imageExtent = capabilies.minImageExtent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
		swapchainCreateInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		vkCreateSwapchainKHR(vulkanData.device, &swapchainCreateInfo, nullptr, &swapchain);
		vulkanData.windowToSwapchain[window] = swapchain;

		VkImage images[2];
		vkGetSwapchainImagesKHR(vulkanData.device, swapchain, &swapchainCreateInfo.minImageCount, images);
#ifdef __linux__
		vulkanData.windowToSwapchainImageViews[window][0] = makeImageView(images[0], swapchainFormat);
		vulkanData.windowToSwapchainImageViews[window][1] = makeImageView(images[1], swapchainFormat);
#else 
		vulkanData.windowToSwapchainImageViews[window][0] = makeImageView(images[0], swapchainFormat);
		vulkanData.windowToSwapchainImageViews[window][1] = makeImageView(images[1], swapchainFormat);
#endif
		vulkanData.windowToSwapchainShaderImageViews[window][0] = makeImageView(images[0], VK_FORMAT_R8G8B8A8_UNORM);
		vulkanData.windowToSwapchainShaderImageViews[window][1] = makeImageView(images[1], VK_FORMAT_R8G8B8A8_UNORM);
	}

	void DestroySurface(void* window)
	{
		vkDestroyImageView(vulkanData.device,vulkanData.windowToSwapchainImageViews[window][0],nullptr);
		vkDestroyImageView(vulkanData.device,vulkanData.windowToSwapchainImageViews[window][1],nullptr);
		vkDestroyImageView(vulkanData.device,vulkanData.windowToSwapchainShaderImageViews[window][0],nullptr);
		vkDestroyImageView(vulkanData.device,vulkanData.windowToSwapchainShaderImageViews[window][1],nullptr);
		vkDestroySurfaceKHR(vulkanData.instance, vulkanData.windowToSurface[window], nullptr);
		vkDestroySwapchainKHR(vulkanData.device, vulkanData.windowToSwapchain[window], nullptr);
	}

	void CreateBuffer(int size, VkBufferUsageFlags bufferFlags, void* mapped, void** buffer) {
	
	};
	void CreateImage(int x, int y, void* mapped, void** image) {
	
	};
	
}
