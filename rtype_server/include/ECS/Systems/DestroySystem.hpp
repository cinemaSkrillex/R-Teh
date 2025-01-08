#include "ECS/Components/Destroyable.hpp"

namespace RealEngine {

class DestroySystem {
   public:
    DestroySystem()  = default;
    ~DestroySystem() = default;
    void update(Registry& registry, float deltaTime) {
        auto entities = registry.view<Destroyable>();

        for (auto entity : entities) {
            auto* destroyable = registry.get_component<Destroyable>(entity);
            if (!destroyable) continue;

            bool shouldDestroy = false;

            switch (destroyable->conditionType) {
                case DestroyConditionType::COORDINATES: {
                    auto* position = registry.get_component<RealEngine::Position>(entity);
                    if (position && position->x == destroyable->data.targetCoordinates.x &&
                        position->y == destroyable->data.targetCoordinates.y) {
                        shouldDestroy = true;
                    }
                    break;
                }
                case DestroyConditionType::RECTANGLE: {
                    auto* position = registry.get_component<RealEngine::Position>(entity);
                    if (position && (position->x < destroyable->data.boundingRect.left ||
                                     position->x > destroyable->data.boundingRect.left +
                                                       destroyable->data.boundingRect.width ||
                                     position->y < destroyable->data.boundingRect.top ||
                                     position->y > destroyable->data.boundingRect.top +
                                                       destroyable->data.boundingRect.height)) {
                        shouldDestroy = true;
                    }
                    break;
                }
                case DestroyConditionType::TIMER: {
                    destroyable->data.timer -= deltaTime;
                    if (destroyable->data.timer <= 0.0f) {
                        shouldDestroy = true;
                    }
                    break;
                }
                case DestroyConditionType::NONE:
                    break;
            }

            // Vérifie une condition personnalisée, si elle existe
            if (!shouldDestroy && destroyable->customCondition) {
                shouldDestroy = destroyable->customCondition();
            }

            // Si la condition est remplie, supprime l'entité
            if (shouldDestroy) {
                registry.kill_entity(entity);
            }
        }
    }
};

}  // namespace RealEngine
