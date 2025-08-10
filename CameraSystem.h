#pragma once
#include "Components.h"
#include <glfw/glfw3.h>
namespace CameraSystem {
	void Init();
	void ProcessInput(GLFWwindow * window);
	void ProcessMouse(GLFWwindow* window, double x_pos, double y_pos);
	CameraComponent * GetMainCamera();
}