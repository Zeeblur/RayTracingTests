#pragma once

#include <memory>
#include <vector>
#include "subsystem.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <fstream>
#include <array>

// if debugging - do INSTANCE validation layers
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// struct for queues
struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

// struct to pass the swapchain details 
struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

// read in binaries/ shader SPIR-V files
static std::vector<char> readFile(const std::string& filename)
{
	// ate - start reading at the end of file to allocate a buffer for the size of the text
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("cannot open file! " + filename);

	// allocate size
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	// start at begining and fill buffer up to stream count (file size)
	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

// structs to store vertex attributes
struct Vertex
{
	glm::vec3 pos;
	glm::vec3 colour;
	glm::vec2 texCoord;

	// how to pass to vertex shader
	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription = {};

		// 1 binding as all data is in 1 array. binding is index.
		// stride is bytes between entries (in this case 1 whole vertex struct)
		// move to the next data entry after each vertex (not instanced rendering)
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;


		return bindingDescription;
	}

	// get attribute descriptions...
	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescription()
	{
		// 2 attributes (position and colour) so two description structs
		std::array<VkVertexInputAttributeDescription, 3> attributeDesc = {};

		attributeDesc[0].binding = 0; // which binding (the only one created above)
		attributeDesc[0].location = 0; // which location of the vertex shader
		attributeDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT; // format as a vector 3 (3floats)
		attributeDesc[0].offset = offsetof(Vertex, pos); // calculate the offset within each Vertex

		// as above but for colour
		attributeDesc[1].binding = 0;
		attributeDesc[1].location = 1;
		attributeDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT; // format as a vector 3 (3floats)
		attributeDesc[1].offset = offsetof(Vertex, colour);

		// texture layout
		attributeDesc[2].binding = 0;
		attributeDesc[2].location = 2;
		attributeDesc[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDesc[2].offset = offsetof(Vertex, texCoord);

		return attributeDesc;
	}
};


// hard code some data (interleaving vertex attributes)
const std::vector<Vertex> vertices = {
	{ { -0.5f, -0.5f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f } },
	{ { 0.5f, 0.5f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f } },
	{ { -0.5f, 0.5f, 0.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },

	{ { -0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
	{ { 0.5f, -0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f } },
	{ { 0.5f, 0.5f, -0.5f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f } },
	{ { -0.5f, 0.5f, -0.5f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } }
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4
};

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

// contains everything needed for a vulkan rendering platform.
class vulkan_platform : public subsystem
{
public:
	inline static std::shared_ptr<vulkan_platform> get()
	{
		std::shared_ptr<vulkan_platform> instance(new vulkan_platform);
		return instance;
	}

	bool initialise();
	bool load_content();
	void update(float delta_time);
	void render();
	void unload_content();
	void shutdown();

	bool amdGPU = false;

private:

	VkInstance instance;
	VkDebugReportCallbackEXT callback;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;

	// frmae buffer stuff
	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkImageView depthImageView;
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;

	VkRenderPass renderPass;		// colour and depth buffer attachment information. 
	VkPipelineCache pipeCache;

	VkDescriptorSetLayout descriptorSetLayout; // currently 2 attachments ubo and texture sampler
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;


	// TODO: this needs to be modified to accept different pools etc
	VkCommandPool gfxCommandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

	// buffers TODO: update this
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	VkBuffer uniformBuffer;
	VkDeviceMemory uniformBufferMemory;

	VkDescriptorPool descriptorPool;
	VkDescriptorSet descriptorSet;

	//Vulkan Instance methods
	void createInstance();
	void setupDebugCallback();
	void createSurface();
	void pickPhysicalDevice(const bool AMD); // gpu choice
	bool isDeviceSuitable(VkPhysicalDevice device, const bool AMD);
	void createLogicalDevice();
	void createSwapChain();

	// depth buffer creation
	//void createDepthResources();
	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);



	// things that may need to be altered.
	void createImageViews();
	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers(); // here
	void createSemaphores();
	void recreateSwapChain();




	void cleanup();
	void cleanupSwapChain();

	/// *** Queues *** ///

	// find queues
	QueueFamilyIndices findQueuesFamilies(VkPhysicalDevice device);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	/// *** Validation Layers *** ///

	bool checkValidationLayerSupport();
	std::vector<const char*> getExtensions();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	
	// create reportcall
	VkResult CreateDebugReportCallbackEXT(
		VkInstance instance,
		const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugReportCallbackEXT* pCallback);

	// call back from validation layers to program
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData);

	// cleanup mem
	void DestroyDebugReportCallbackEXT(VkInstance instance,
		VkDebugReportCallbackEXT callback,
		const VkAllocationCallbacks* pAllocator);

};
