#pragma once
#include "globals.h"
#include "vkGraphics.h"
#include "filesystem/resource_loader.h"

class Engine
{
public:
	void Run(vector<string> args);
private:
	VkGraphics graphics;
	void Init();
	void MainLoop();
	void Cleanup();
};

