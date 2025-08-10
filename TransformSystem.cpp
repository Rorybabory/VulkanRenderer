#include "TransformSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

void TransformSystem::GenerateTransformMatrix(TransformComponent* transform) 
{
	transform->modelMatrix = glm::identity<glm::mat4>();
	transform->modelMatrix = glm::translate(transform->modelMatrix, transform->pos);
	transform->modelMatrix = glm::scale(transform->modelMatrix, transform->scale);
	transform->modelMatrix = glm::rotate(transform->modelMatrix, transform->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform->modelMatrix = glm::rotate(transform->modelMatrix, transform->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform->modelMatrix = glm::rotate(transform->modelMatrix, transform->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
}

void TransformSystem::Update(entt::registry& registry)
{
	auto view = registry.view<TransformComponent>();

	for (auto [entity, transform] : view.each()) {
		GenerateTransformMatrix(&transform);
	}
}

void TransformSystem::CreateTransform(TransformComponent& transform) {
	transform.pos = glm::vec3(0.0);
	transform.scale = glm::vec3(1.0f);
	transform.rotation = glm::vec3(0.0f);
}

void TransformSystem::SpinTransform(TransformComponent* transform) {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	transform->rotation.y = time * glm::radians(35.0f);
	transform->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}