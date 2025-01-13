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

void GameInstance::spawnMob(const std::string& mobName, const sf::Vector2f& position, float angle) {
    float level_speed = 1.0f;
    if (mobName == "directional_canon") {
        rtype::DirectionalCanon directionalCanon(_registry, position, level_speed);
        addAndGetEnemy(directionalCanon.getEntity());
    } else if (mobName == "eye_bigion") {
        rtype::EyeBigion eyeBigion(_registry, position);
        addAndGetEnemy(eyeBigion.getEntity());
    } else if (mobName == "eye_boss") {
        rtype::EyeBoss eyeBoss(_registry, position);
        addAndGetEnemy(eyeBoss.getEntity());
    } else if (mobName == "eye_bomber") {
        rtype::EyeBomber eyeBomber(_registry, position);
        addAndGetEnemy(eyeBomber.getEntity());
    } else if (mobName == "eye_minion") {
        rtype::EyeMinion eyeMinion(_registry, position);
        addAndGetEnemy(eyeMinion.getEntity());
    } else if (mobName == "mob_spawner_ship") {
        rtype::MobSpawnerShip mobSpawnerShip(_registry, position);
        addAndGetEnemy(mobSpawnerShip.getEntity());
    } else if (mobName == "mortar") {
        rtype::Mortar mortar(_registry, position);
        addAndGetEnemy(mortar.getEntity());
    } else if (mobName == "robot_boss_minion") {
        rtype::RobotBossMinion robotBossMinion(_registry, position);
        addAndGetEnemy(robotBossMinion.getEntity());
    } else if (mobName == "robot_mini_boss") {
        rtype::RobotMiniBoss robotMiniBoss(_registry, position);
        addAndGetEnemy(robotMiniBoss.getEntity());
    } else if (mobName == "space_drill") {
        rtype::SpaceDrill spaceDrill(_registry, position);
        addAndGetEnemy(spaceDrill.getEntity());
    } else if (mobName == "space_plane") {
        rtype::SpacePlane spacePlane(_registry, position);
        addAndGetEnemy(spacePlane.getEntity());
    } else if (mobName == "space_sphere") {
        rtype::SpaceSphere spaceSphere(_registry, position);
        addAndGetEnemy(spaceSphere.getEntity());
    } else if (mobName == "wall_turret") {
        rtype::WallTurret turretCanon(_registry, position, 0, level_speed);
        addAndGetEnemy(turretCanon.getCannonEntity());
        addAndGetEnemy(turretCanon.getPedestalEntity());
    }
}