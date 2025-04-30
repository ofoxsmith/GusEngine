#pragma once
#include "globals.h"
#include "renderer/vkGraphics.h"

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

