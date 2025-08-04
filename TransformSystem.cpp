#include "TransformSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void TransformSystem::GenerateTransformMatrix(TransformComponent* transform) 
{
	transform->modelMatrix = glm::identity<glm::mat4>();
	transform->modelMatrix = glm::scale(transform->modelMatrix, transform->scale);
	transform->modelMatrix = glm::rotate(transform->modelMatrix, transform->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform->modelMatrix = glm::rotate(transform->modelMatrix, transform->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform->modelMatrix = glm::rotate(transform->modelMatrix, transform->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	transform->modelMatrix = glm::translate(transform->modelMatrix, transform->pos);
}

void TransformSystem::Update(entt::registry& registry)
{
	auto view = registry.view<TransformComponent>();

	for (auto [entity, transform] : view.each()) {
		GenerateTransformMatrix(&transform);
	}
}