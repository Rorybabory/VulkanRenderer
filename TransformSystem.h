#pragma once
#include "Components.h"
#include <entt.hpp>
namespace TransformSystem {
	void GenerateTransformMatrix(TransformComponent * transform);
	void Update(entt::registry &registry);
	void CreateTransform(TransformComponent& transform);
	void SpinTransform(TransformComponent* transform);
}