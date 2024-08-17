#include "engine.h"

void Engine::Run(vector<string> args) {
	Init();
	ResourceLoader::Load("://shaders/shader.vert");
	MainLoop();
	Cleanup();
}

void Engine::Init() {
	graphics.Init();
}

void Engine::MainLoop() {
	graphics.MainLoop();
}

void Engine::Cleanup() {
	Log.Info("Core", "Cleaning up resources");
	graphics.Cleanup();
	Log.Info("Core", "Exiting");
}