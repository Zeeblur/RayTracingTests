#include "engine.h"
#include "thread_pool.h"

#include <iostream>
#include <chrono>

#include <sstream>
#include <iomanip>
#include <fstream>
using namespace glfw;

void engine::add_subsystem(const std::string &name, std::shared_ptr<subsystem> sys)
{
    _subsystems[name] = sys;
}

std::shared_ptr<subsystem> engine::get_subsystem(const std::string &name)
{
    auto found = _subsystems.find(name);
    if (found == _subsystems.end())
        return nullptr;
    return found->second;
}

// Runs the engine.  Note that this technique takes no account of subsystem order.
// If subsystem order is important consider using another mechanism.
void engine::run()
{
    initGraphics();
    initSubsystems();
    mainLoop();
    cleanup();
}

void engine::initWindowMan()
{	
	// insert resolution here
	glfw::runWindow(600, 400);
}

void engine::initGraphics()
{
	// initialise platform here
	vulkan_platform::get()->initialise();
}

void engine::initSubsystems()
{

    // Initialise all the subsystems
    for (auto &sys : _subsystems)
    {
        // If initialise fails exit run.
        if (!sys.second->initialise())
        {
            return;
        }
    }

    // Load content for all the subsystems
    for (auto &sys : _subsystems)
    {
        // If load_content fails exit run.
        if (!sys.second->load_content())
        {
            return;
        }
    }
}

void engine::mainLoop()
{
    // Loop until not running.
    // Check if the ESC key was pressed or the window was closed

	float dt = 1 / 60.0;

	auto currentTime = std::chrono::system_clock::now();

    while (_running && glfwWindowShouldClose(window) == 0)
    {
        //std::cout << "Engine Running" << std::endl;
		auto newTime = std::chrono::system_clock::now();

		float frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - currentTime).count();
		currentTime = newTime;

		// variable delta time
		float deltaTime = frameTime / 1000.0f; // std::min (dt) ?
		// Update the subsystems. 
		for (auto &sys : _subsystems)
			if (sys.second->get_active())
				sys.second->update(deltaTime);

        while(thread_pool::get()->jd_.jobs_.size() != 0)
        {
            // wait on the threads to finish
        }

        // Render the subsystems.
        for (auto &sys : _subsystems)
            if (sys.second->get_visible())
                sys.second->render();

        while(thread_pool::get()->jd_.jobs_.size() != 0)
        {
            // wait on the threads to finish
        }
    }

	engine::get()->set_running(false);
}

void engine::cleanup()
{
    // Unload the content.
    for (auto &sys : _subsystems)
    {
        sys.second->unload_content();
    }

    // Shutdown subsystems
    for (auto &sys : _subsystems)
    {
        sys.second->shutdown();
    }


    // Clear out all the subsystems causing destructors to call.
    _subsystems.clear();

    thread_pool::get()->shutdown();

    // Engine will now exit.
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}




