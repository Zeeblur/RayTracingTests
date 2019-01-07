#include "engine.h"
#include "subsystem.h"
#include <chrono>
#include <iostream>

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

void engine::run()
{
	init_subsystem();
	main_loop();
	cleanup();
}

void engine::init_subsystem()
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

void engine::main_loop()
{
	// Loop until not running.
	// Check if the ESC key was pressed or the window was closed
	float dt = 1 / 60.0;

	auto currentTime = std::chrono::system_clock::now();

	while (_running && glfwWindowShouldClose(window) == 0)
	{

		glfwPollEvents();
				
		std::cout << "Engine Running" << std::endl;
		auto newTime = std::chrono::system_clock::now();

		float frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - currentTime).count();
		currentTime = newTime;

		// variable delta time
		float deltaTime = frameTime / 1000.0f; // std::min (dt) ?
		// Update the subsystems. 
		for (auto &sys : _subsystems)
			if (sys.second->get_active())
				sys.second->update(deltaTime);

		// Render the subsystems.
		for (auto &sys : _subsystems)
		   if (sys.second->get_visible())
		        sys.second->render();
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

	

	// Engine will now exit.
	// Close OpenGL window and terminate GLFW
	//glfwTerminate();
}
