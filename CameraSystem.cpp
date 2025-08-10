#include "CameraSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RenderConstants.h"
#include <chrono>
#include <iostream>
namespace CameraSystem {
	CameraComponent mainCamera;
}

void UpdateCameraTransform(CameraComponent& camera) {
	camera.view = glm::lookAt(camera.pos, camera.pos + camera.forward, camera.up);
	camera.proj = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
}

void CameraSystem::Init() {

	mainCamera.pos = glm::vec3(2.0f, 2.0f, 2.0f);
	mainCamera.forward = glm::vec3(-1.0f, -0.5f, -1.0f);
	mainCamera.up = glm::vec3(0.0f, 1.0f, 0.0f);

	UpdateCameraTransform(mainCamera);
}
CameraComponent* CameraSystem::GetMainCamera() {
	return &mainCamera;
}
void CameraSystem::ProcessInput(GLFWwindow* window) {
	const float cameraSpeed = 5.0f;

	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	static float lastTime = 0.0f;
	float dt = time - lastTime;
	lastTime = time;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
		mainCamera.pos += cameraSpeed * mainCamera.forward * dt;
	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
		mainCamera.pos -= cameraSpeed * mainCamera.forward * dt;
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mainCamera.pos -= glm::normalize(glm::cross(mainCamera.forward, mainCamera.up)) * cameraSpeed * dt;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mainCamera.pos += glm::normalize(glm::cross(mainCamera.forward, mainCamera.up)) * cameraSpeed * dt;
	UpdateCameraTransform(mainCamera);
}
const float sensitivity = 0.1f;

void CameraSystem::ProcessMouse(GLFWwindow* window, double x_pos, double y_pos) {
	static float lastX = x_pos, lastY = y_pos;
	static float yaw = -140.0f, pitch = -22.0f;

	float xOffset = (float)x_pos - lastX;
	float yOffset = (float)y_pos - lastY;
	lastX = (float)x_pos;
	lastY = (float)y_pos;

	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch -= yOffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 dir;
	dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	dir.y = sin(glm::radians(pitch));
	dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	mainCamera.forward = glm::normalize(dir);
	std::cout << "pitch: " << pitch << " yaw: " << yaw << "\n";
}