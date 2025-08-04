
#include <iostream>
#include "Renderer.h"
#include "AssetManager.h"
#include "CameraSystem.h"
#include "TransformSystem.h"
#include <entt.hpp>
int main() {
    Renderer * renderer;
    entt::registry registry;
    try {
        Model model = AssetManager::LoadModel("models/aliensoldier.obj");
        
        renderer = new Renderer();
        renderer->SetModel(model);
        renderer->Init();
        CameraSystem::Init();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        while (!renderer->Update()) {
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