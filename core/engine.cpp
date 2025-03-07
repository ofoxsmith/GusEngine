#include "engine.h"
#include "filesystem/resource_loader.h"
#include "types/type_registry.h"

#include "filesystem/engine_io.h"

void Engine::Run(vector<string> args) {
	engine_type_registry::type_registry::register_all_types();

	Resource x = *EngineIO::ObjectLoader::LoadSerialisedResourceBinary("output.bin");
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