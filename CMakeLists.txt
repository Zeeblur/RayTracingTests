cmake_minimum_required (VERSION 2.6)
project (RayTrace1Wkend)

#set c++ standard
set (CMAKE_CXX_STANDARD 11)

# add the binary tree to the search path for include files
include_directories ("${PROJECT_BINARY_DIR}")

SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

include(DownloadProject.cmake)

# don't download glfw project shit
option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_DOCUMENT_INTERNALS "" OFF)
option(GLFW_INSTALL "" OFF)

#get glfw
download_project(PROJ glfw
  GIT_REPOSITORY      https://github.com/glfw/glfw.git
  GIT_TAG             0f488ac
  ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
include_directories("${glfw_SOURCE_DIR}/include")

#find vulkan sdk
include_directories("$ENV{VULKAN_SDK}/include")
link_directories("$ENV{VULKAN_SDK}/lib")


#get glm
download_project(PROJ glm
  GIT_REPOSITORY      https://github.com/g-truc/glm.git
  GIT_TAG             6a1e2ec
  ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)
add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
include_directories("${glm_SOURCE_DIR}")



# add the exes


# ray TEST:
file(GLOB_RECURSE SOURCESRAY src/raytracing/*.cpp src/raytracing/*.h external/*.h)

add_executable(raytest ${SOURCESRAY})
target_link_libraries (raytest  ${EXTRA_LIBS})

file(GLOB_RECURSE SOURCESPLAT src/platform/*.cpp src/platform/*.h external/*.h)

add_executable(rayplatform ${SOURCESPLAT})
target_link_libraries (rayplatform  ${EXTRA_LIBS})
target_link_libraries (rayplatform glfw)
target_link_libraries (rayplatform vulkan-1)


  
add_custom_target(copy_res ALL COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/res" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/res") 
