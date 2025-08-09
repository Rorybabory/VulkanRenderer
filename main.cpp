
#include <iostream>
#include "Renderer.h"
#include "AssetManager.h"
#include "CameraSystem.h"
#include "TransformSystem.h"
#include <entt.hpp>

entt::entity CreateEntity(entt::registry& registry, Renderer* renderer, std::string modelPath, std::string texturePath, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), float rot = 0.0f) {
    entt::entity ent = registry.create();

    Texture texture;
    renderer->CreateTexture(texture, texturePath);


    TransformComponent& transform = registry.emplace<TransformComponent>(ent);
    GeometryComponent& geometry = registry.emplace<GeometryComponent>(ent);
    UniformComponent& uniform = registry.emplace<UniformComponent>(ent);
    TransformSystem::CreateTransform(transform);
    renderer->CreateGeometry(geometry,modelPath);
    uniform.pipeline = &renderer->GetMainPipeline();
    uniform.texture = texture;
    UniformSystem::CreateDescriptorSets(&uniform);
    transform.pos = pos;
    transform.rotation.y = rot;
    return ent;
}

int main() {
    Renderer * renderer;
    entt::registry registry;
    try {
        renderer = new Renderer(registry);
        renderer->Init();
        CameraSystem::Init();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    //entt::entity alien_soldier = CreateEntity(registry, renderer, "models/aliensoldier.obj", "textures/aliensoldier.png");
    entt::entity chair = CreateEntity(registry, renderer, "models/chair.obj", "textures/wood.png", glm::vec3(-1.5f, 0.0f, 0.0f), 0.4f);
    entt::entity chair2 = CreateEntity(registry, renderer, "models/chair.obj", "textures/wood.png", glm::vec3(0.4f, 0.0f, -0.7f), 0.7f);
    entt::entity chair3 = CreateEntity(registry, renderer, "models/chair.obj", "textures/wood.png", glm::vec3(-1.0f, 0.0f, -1.8f), 2.3f);
    entt::entity chair4 = CreateEntity(registry, renderer, "models/chair.obj", "textures/wood.png", glm::vec3(0.1f, 0.0f, 0.1f), 0.1f);

    entt::entity floor = CreateEntity(registry, renderer, "models/floor.obj", "textures/stone.png");

    TransformComponent& transform = registry.get<TransformComponent>(chair);

    try {
        while (!renderer->Update()) {
            //TransformSystem::SpinTransform(&transform);
            TransformSystem::SpinTransform(&transform);
            TransformSystem::Update(registry);
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        delete renderer;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}