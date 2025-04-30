#pragma once
#include "globals.h"
#include "renderer/renderer.h"

class Engine
{
public:
	void Run(vector<string> args);
private:
	Renderer graphics;
	void Init();
	void MainLoop();
	void Cleanup();
};

