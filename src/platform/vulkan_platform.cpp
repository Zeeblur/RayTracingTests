#pragma once


// contains everything needed for a vulkan rendering platform.

#include "vulkan_platform.h"
#include <iostream>
#include <set>
#include <algorithm>
#include "glfw.h"
#include <array>



bool vulkan_platform::initialise()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::cout << extensionCount << " extensions supported" << std::endl;


	createInstance();
	setupDebugCallback();
	createSurface();
	pickPhysicalDevice(amdGPU);
	createLogicalDevice();
	createSwapChain();

	createImageViews();
	createRenderPass();

	createDescriptorSetLayout();
	createGraphicsPipeline();
	createCommandPool();
	createDepthResources();
	createFramebuffers();
	createTextureImage();
	createTextureImageView();
	createTextureSampler();

	return true;
}

bool vulkan_platform::load_content() { return true; }
void vulkan_platform::update(float delta_time) {}
void vulkan_platform::render() {}
void vulkan_platform::unload_content() {}
void vulkan_platform::shutdown() {}


/// **** validation layer supports

// check if validation layers are supported by the gpu
bool vulkan_platform::checkValidationLayerSupport()
{
	// find layers.
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	// if layers are avalible, store them.
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// for each layer check if it exists in the list.
	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
			return false;
	}

	return true;
}

// check for extensions and return a list (based on if validation layers are enabled)
std::vector<const char*> vulkan_platform::getExtensions()
{
	std::vector<const char*> extensions;

	// find extensions
	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++)
	{
		extensions.push_back(glfwExtensions[i]);
	}

	if (enableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	return extensions;
}

// set up callbacks if needed
void vulkan_platform::setupDebugCallback()
{
	// not enabled so don't initialise
	if (!enableValidationLayers) return;

	// set the info
	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = debugCallback;

	// create the callback
	if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
		throw std::runtime_error("failed to set up debug callback!");


}


VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_platform::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char * layerPrefix, const char * msg, void * userData)
{
	std::cerr << "validation layer: " << msg << std::endl;

	// VkDebugReportFlagsEXT - type of message.
	// objtype - stores enum to device subject of the message (obj)
	// msg is pointer to itself.
	// userdata to pass data to the callback.

	// you should always return false, as it only returns true if a call has been aborted.
	return VK_FALSE;
}

// have to look up callback address using vkGetInstanceProcAddr. create own proxy function that handles this in the background
VkResult vulkan_platform::CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

	if (func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pCallback);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

// clear memory
void vulkan_platform::DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

	if (func != nullptr)
		func(instance, callback, pAllocator);

}

/// **** validation layer supports end


// create a struct with driver specific details
void vulkan_platform::createInstance()
{
	// check if validation layers exist
	if (enableValidationLayers && !checkValidationLayerSupport())
		throw std::runtime_error("validation layers requested, but not available!");


	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "RayTracer";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Zoe's Engine 2.0";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// to tell the driver which global ext and validation layers to use.
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// create extensions and set info
	auto extensions = getExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();


	// get/set validation layers if flag true
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	// try creating instance
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		throw std::runtime_error("Failed to create vulkan instance");

}



/// ** creating the devices


// select GPU that supports the features needed
void vulkan_platform::pickPhysicalDevice(const bool AMD)
{
	// find devices and store in vector
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);


	// create vector and store
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	if (deviceCount == 0)
		throw std::runtime_error("Failed to find GPU with Vulkan support!");

	for (const auto& dev : devices)
	{
		std::cout << "checking device" << std::endl;

		if (isDeviceSuitable(dev, AMD))
		{
			physicalDevice = dev;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE)
		throw std::runtime_error("failed to find a suitable GPU!");

	std::cout << "device suitable" << std::endl;
}

// check if all extensions include required ones
bool vulkan_platform::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	// get extensions
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	// create list of availble extensions
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

// check for device capability
bool vulkan_platform::isDeviceSuitable(VkPhysicalDevice device, const bool AMD)
{
	// check for physical properties
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures = {};
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	amdGPU = AMD;

	// Don't choose 1080
	if (AMD)
	{
		char *output = NULL;
		output = strstr(deviceProperties.deviceName, "GTX");
		if (output)
		{

			printf("1080 Found - Skipping\n");
			return false;
		}
	}

	// is this a discrete gpu and does it have geom capabilities 
	bool physical = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		deviceFeatures.geometryShader;

	QueueFamilyIndices indices = findQueuesFamilies(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	//timestampPeriod = deviceProperties.limits.timestampPeriod; for timing
	return physical && indices.isComplete() && extensionsSupported && swapChainAdequate && deviceFeatures.samplerAnisotropy;
}

// find and return the queuefamilies that support graphics & presentation support
QueueFamilyIndices vulkan_platform::findQueuesFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	// as before, find them, set them
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	// find suitable family that supports graphics.
	unsigned int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{

		// check for presentation support to window
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (queueFamily.queueCount > 0 && presentSupport)
			indices.presentFamily = i;


		// check for drawing command support
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphicsFamily = i;

		if (indices.isComplete())
			break;

		i++;
	}

	return indices;
}

// check swapchain support for surface format, presentation mode, swap extent.
SwapChainSupportDetails vulkan_platform::querySwapChainSupport(VkPhysicalDevice device) {
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

// create a logic device + queues
void vulkan_platform::createLogicalDevice()
{
	// specify the queues to create
	// returns indicies of queues that can support graphics calls
	QueueFamilyIndices indices = findQueuesFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQFamilies = { indices.graphicsFamily, indices.presentFamily };


	// influence sheduling of the command buffer (NEEDED EVEN IF ONLY 1 Q)
	float queuePriority = 1.0f;

	// for each unique queue family, create a new info for them and add to list
	for (int queueFam : uniqueQFamilies)
	{
		VkDeviceQueueCreateInfo qCreateInfo;
		qCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

		// this is either graphics/surface etc
		qCreateInfo.queueFamilyIndex = queueFam;

		qCreateInfo.pQueuePriorities = &queuePriority;
		qCreateInfo.queueCount = 1;

		// add to list
		queueCreateInfos.push_back(qCreateInfo);
	}

	// define features wanted to use **
	VkPhysicalDeviceFeatures deviceFeatures = {};

	// create info for the logical device
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	// add queue info and devices
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;

	// swap chain extensions enable!
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	// create validation
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	// instantiate object including the phys device to interface to.
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
		throw std::runtime_error("Failed to create logical device!");

	vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);

}


/// *** Swapchain/present/image views formats *** ///

// format: contains a format (Specifying the colour channels and types) and colourspace.
VkSurfaceFormatKHR vulkan_platform::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	// use SRGB for colour space - more accurate percieved colours. 
	// But use standard RGB for the colour format.

	// if surface has no prefered format then it is set as undefined. (therefore free to choose any)
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };


	// if not, search for avalible formatrs.
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return availableFormat;
	}

	// if none is found that is suitable, just use the first one.
	return availableFormats[0];
}

VkFormat vulkan_platform::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

VkFormat vulkan_platform::findDepthFormat()
{
	return findSupportedFormat
	(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

// Actual conditions for displaying to the screen !!!!
VkPresentModeKHR vulkan_platform::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
	// this is always guaranteed to be avalible. -- SIMILAR TO VSYNC. - takes a frame when the display is refreshed and the program adds frames to the end of the queue.
	// blocks application when display queue is full (double buffering)
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentMode : availablePresentModes) {

		// triple buffering!! - avoid tearing less than std vsync. 
		// doesn't block app when queue is full, the images are replaced with newer ones. (Frames = images)
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
		// immediately transfered to screen when rendered - could cause TEARING
		// prefer this over fifo as some drivers don't support it properly.
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			bestMode = availablePresentMode;
		}

	}

	return bestMode;
}

// Swap extent is the resolution of the swap chain images - try to match the window
VkExtent2D vulkan_platform::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	int width, height;
	glfwGetWindowSize(glfw::window, &width, &height);

	// some wm allow whatever
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	// clamp the values to the max and min values of the capabilities.
	else
	{
		VkExtent2D actualExtent = { width, height };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

// wrapper to pass the shader code to the pipeline
VkShaderModule vulkan_platform::createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();

	// specify a pointer to the buffer with the code and the length of it. need to cast as it's in bytecode size
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	// create the module
	VkShaderModule shaderModule;

	// check it's created
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		throw std::runtime_error("Failed to create shader module!");

	return shaderModule;
}

// create surface for interfacing with GLFW to render things on screen
void vulkan_platform::createSurface()
{
	if (glfwCreateWindowSurface(instance, glfw::window, nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("Failed to create glfw surface.");
}

// create swapchain
void vulkan_platform::createSwapChain()
{
	// get capabilities
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

	// choose from valid swapchain modes
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	// set the amount of images in the swap chain (queue length)
	// try to get the min needed to render properly and have 1 more than that for triple buffering
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	// if it's not zero and higher than the max... change it to max
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}


	// create info for swapchain
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	// which surface to tie to
	createInfo.surface = surface;

	// details chosen
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;

	// image layers always 1 unless stereoscopic image
	createInfo.imageArrayLayers = 1;

	// transfer to colour - not to a texture - no post-processing for now.
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


	// specify how to handle images from different queues.
	QueueFamilyIndices indices = findQueuesFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };

	// specify how the data is shared bewteen multiple queues
	if (indices.graphicsFamily != indices.presentFamily)
	{
		// no explicit ownership transfers - images are shared.
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;

		// specify which queue families own/share this.
		createInfo.queueFamilyIndexCount = sizeof(queueFamilyIndices) / sizeof(queueFamilyIndices[0]);
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else // if the graphics and present family are the same - stick to exclusive.
	{
		// An image is owned by one queue family at a time and ownership must be explicitly transfered before using it in another queue family.
		//This option offers the best performance.
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	// can transform image if needed.
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

	// alpha blending layers -- opaque ignores this
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	// doesn't care about pixels that are obsured by something
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;



	// null for now - but this stores old chain for when it needs to be re-created entirely due to a window resize or something.
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	// create the damn thing!  -- the logical device, swap chain creation info, optional custom allocators and a pointer to the variable to store the handle in
	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create swap chain!");
	}

	// retrieve handles to swapchain imgs
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

}

// create image views for the swapchain images so they can be used in the render pipeline.
void vulkan_platform::createImageViews()
{
	// resize list to store all created images
	swapChainImageViews.resize(swapChainImages.size());

	// iterate through images
	for (size_t i = 0; i < swapChainImages.size(); ++i)
	{
		// As normal, createinfo for the object
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];

		// specify how to treat images 1d/2d/3d etc
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;

		// default colour mapping
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		// what is the image's purpose - which part should be accessed (for this just colour targets for rendering) - no mipmaps or layers
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		// create the view and store in the list.
		if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create image views!");

	}
}


/// *** render passes *** ///

// render pass - specifies buffers and samples 
void vulkan_platform::createRenderPass()
{
	// create a single colour buffer represented by a swap chain image
	VkAttachmentDescription colourAttachment = {};
	colourAttachment.format = swapChainImageFormat;
	colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // not multisampling 

	// clear buffer at the start
	colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	// store colour buffer memory to be able to access later
	colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	// if using a stencil buffer
	colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	// don't care what the initial image was layed out. but after render pass make it ready for presentation
	colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// dpeth attachment
	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// Subpasses and attachments.
	// 1 render pass can be multiple subpasses (i.e post processing)
	VkAttachmentReference colourAttachmentRef = {};
	colourAttachmentRef.attachment = 0;
	colourAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// just do 1 subpass for now specify the colour attachgment
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colourAttachmentRef;   // INDEX OF THIS IS THE LAYOUT LOCATION IN THE SHADER
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	// create subpass dependencies 
	// specify operations to wait on and where they occur. - need to wait for the swap chain to finish reading before accessing it
	// so wait for the colour attachment stage
	// this prevents the render pass from transistioning before we need to start writing colours to it

	// array of depthbuffer
	std::array<VkAttachmentDescription, 2> attachments = { colourAttachment, depthAttachment };

	//// CREATE RENDER PASS

	// Subpass dependencies for layout transitions
	std::array<VkSubpassDependency, 2> dependencies;

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
		throw std::runtime_error("Failed to create Render Pass!");


}



void vulkan_platform::cleanupSwapChain()
{
	//vkDestroyImageView(device, depthImageView, nullptr);
	//vkDestroyImage(device, depthImage, nullptr);
	//vkFreeMemory(device, depthImageMemory, nullptr);

	//for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
	//{
	//	vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
	//}

	//vkFreeCommandBuffers(device, gfxCommandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

	//vkDestroyPipeline(device, graphicsPipeline, nullptr);
	//vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	//vkDestroyRenderPass(device, renderPass, nullptr);

	//for (size_t i = 0; i < swapChainImageViews.size(); i++)
	//{
	//	vkDestroyImageView(device, swapChainImageViews[i], nullptr);
	//}

	//vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void vulkan_platform::cleanup()
{
	cleanupSwapChain();

	//vkDestroySampler(device, textureSampler, nullptr);
	//vkDestroyImageView(device, textureImageView, nullptr);

	//vkDestroyImage(device, textureImage, nullptr);
	//vkFreeMemory(device, textureImageMemory, nullptr);

	//vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	//vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
	//vkDestroyBuffer(device, uniformBuffer, nullptr);
	//vkFreeMemory(device, uniformBufferMemory, nullptr);

	//vkDestroyBuffer(device, indexBuffer, nullptr);
	//vkFreeMemory(device, indexBufferMemory, nullptr);

	//vkDestroyBuffer(device, vertexBuffer, nullptr);
	//vkFreeMemory(device, vertexBufferMemory, nullptr);

	//vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
	//vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);

	//vkDestroyCommandPool(device, gfxCommandPool, nullptr);

	vkDestroyDevice(device, nullptr);
	DestroyDebugReportCallbackEXT(instance, callback, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

}




