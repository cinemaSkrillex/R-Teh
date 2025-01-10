/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameEntities
*/

#include "../include/Game/GameInstance.hpp"

std::shared_ptr<RealEngine::Entity> GameInstance::addAndGetPlayer(sf::Vector2f position) {
    rtype::PlayerEntity player(_registry, position);
    auto                playerEntity = player.getEntity();
    _players.emplace(*playerEntity, playerEntity);
    return _players.at(*playerEntity);
}

std::shared_ptr<RealEngine::Entity> GameInstance::addAndGetEntity(sf::Vector2f position) {
    auto entity = _registry.spawn_entity();
    _enemies.push_back(entity);
    return entity;
}

std::shared_ptr<RealEngine::Entity> GameInstance::addAndGetBullet(sf::Vector2f position,
                                                                  sf::Vector2f direction,
                                                                  float        speed,
                                                                  std::string  spriteName,
                                                                  float damage, int health) {
    rtype::Bullet bullet(_registry, position, direction, speed, spriteName, damage, health);
    _bullets.push_back(bullet.getEntity());
    return bullet.getEntity();
}

std::shared_ptr<RealEngine::Entity> GameInstance::addAndGetEnemy(
    std::shared_ptr<RealEngine::Entity> enemyEntity) {
    _enemies.push_back(enemyEntity);
    return _enemies.back();
}

void GameInstance::spawnMob(const std::string& mobName, const sf::Vector2f& position) {
    if (mobName == "eye_minion") {
        rtype::EyeMinion eyeMinion(_registry, position);
        addAndGetEnemy(eyeMinion.getEntity());
    } else if (mobName == "space_sphere") {
        rtype::SpaceSphere spaceSphere(_registry, position);
        addAndGetEnemy(spaceSphere.getEntity());
    }
}