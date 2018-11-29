#pragma once


// contains everything needed for a vulkan rendering platform.


#include "vulkan_platform.h"


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>


bool vulkan_platform::initialise()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::cout << extensionCount << " extensions supported" << std::endl;


	return true;
}

bool vulkan_platform::load_content() { return true; }
void vulkan_platform::update(float delta_time) {}
void vulkan_platform::render() {}
void vulkan_platform::unload_content() {}
void vulkan_platform::shutdown() {}