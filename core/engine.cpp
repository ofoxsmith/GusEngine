#include "engine.h"
#include "filesystem/resource_loader.h"

void Engine::Run(vector<string> args) {
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