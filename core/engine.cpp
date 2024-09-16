#include "engine.h"
#include "filesystem/resource_loader.h"
#include "types/type_registry.h"
void Engine::Run(vector<string> args) {
	enum class Testing {
		Ab,
		Bc,
		Cd,
		De
	};
	engine_type_registry::type_registry::class_register_enum<Testing>(-1, "TestEnum");
	return;
	engine_type_registry::type_registry::register_all_types();
	engine_type_registry::type_registry::log_all_type_data_debug();
	Init();
	MainLoop();
	Cleanup();
}

void Engine::Init() {
	ResourceLoader::Init();
	graphics.Init();
}

void Engine::MainLoop() {
	graphics.MainLoop();
}

void Engine::Cleanup() {
	Log.Info("Core", "Cleaning up vulkan");
	graphics.Cleanup();
	Log.Info("Core", "Cleaning up resources");
	ResourceLoader::Cleanup();
	Log.Info("Core", "Exiting");
}