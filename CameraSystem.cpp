#include "CameraSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RenderConstants.h"

namespace CameraSystem {
	CameraComponent mainCamera;
}

void CameraSystem::Init() {
	mainCamera.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mainCamera.proj = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
}
CameraComponent* CameraSystem::GetMainCamera() {
	return &mainCamera;
}