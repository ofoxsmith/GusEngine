#pragma once
#include "globals.h"
#include "renderer/renderer.h"
#include <GLFW/glfw3.h>

class Engine
{
public:
	void Run(vector<string> args);
private:
	GLFWwindow* _window;
	Renderer _renderer;
	bool _framebufferChanged;

	void initWindow();
	static void framebufferResizeCallback(GLFWwindow* window, int32_t width, int32_t height);


	void Init();
	void MainLoop();
	void Cleanup();
};

